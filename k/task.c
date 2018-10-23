
#include <task.h>
#include <timer.h>
#include <panic.h>

// Pool of 16 pointers to task rings
priorityList priListPool[nTaskPriorities];

// Pointer to highest priority ring pointer currently active.
static priorityList *priList;

// Pool of free messages
message messagePool[maxMessages];

// The global freelist pointer within the message class.
message *message::freeList = 0;

// The current task
task *currentTask;

// Global indicating the depth of nesting of ISRs
extern "C" short isrLevel;

// Global indicating that an ISR should task switch on exit.
extern "C" short switchPending;

extern "C" asmSwitchToTask(sp_t *pOldTaskSP, sp_t newTaskSP);

// Start a task running by putting it onto its ring
// This function must be called with interrupts locked out
static void wakeTask(task *ptask)
{
	if (!ptask->onRunList)
	{
		priorityList *p = ptask->pPriorityList;

		if (p->theTask == 0)
		{	// No tasks on this list entry, so it's not in priList
			priorityList *p2 = priList;

			if (p2 == 0)
				PANIC();

			if (p2->priority < p->priority)
			{	// Insert at head
				p->next = priList;
				p->prev = 0;
				priList = p;
			}
			else
			{
				short ppriority = p->priority;
				priorityList *p3;

				while ((p3 = p2->next) && p3->priority > ppriority)
					p2 = p3;

				if (p3 == 0)
				{	// Place at tail
					p2->next = p;
					p->next = 0;
				}
				else
				{	// p2->next->priority <= p->priority
					p3->prev = p;
					p->next = p3;
					p->prev = p2;
					p2->next = p;
				}
			}
			p->theTask = ptask;
			ptask->prev = ptask->next = ptask;
		}
		else
		{	// There are already one or more tasks on this ring
			// Stick this task at the head of the ring (next to get run)
			task *ptask2 = p->theTask;

			ptask->next = ptask2->next;
			ptask->prev = ptask2;
			ptask->next->prev = ptask;
			ptask2->next = ptask;
		}
		ptask->onRunList = 1;
	}
}

// Idle a task by removing it from its ring.
// This function must be called with interrupts locked out
static void idleTask(task *ptask)
{
	if (!ptask->onRunList)
		PANIC();

	if (ptask->next != ptask)
	{	// Not the only task on this ring
		ptask->next->prev = ptask->prev;
		ptask->prev->next = ptask->next;
	}
	else
	{	// The only one on the ring: remove this priority level
		// from priList
		priorityList *p = ptask->pPriorityList;

		p->theTask = 0;			// Indicator that this list entry is not on
								// priList.
		if (priList == p)
		{	// This was the highest priority task
			priList = p->next;
		}
		else
		{	// Now unlink this priority list entry
			// It's not the highest priority entry, so it has a 'prev'
			p->prev->next = p->next;
			if (p->next)
				p->next->prev = p->prev;
		}
	}
	ptask->onRunList = 0;
}

// Switch from 'currentTask' to 'priList->theTask'
// This function must be called with interrupts locked out
extern "C" void nextTask(void)
{
	task *ptask = priList->theTask;
	task *p = currentTask;

	currentTask = ptask;
	asmSwitchToTask(&p->sp, ptask->sp);
}

inline message *
sendMessage(mailBox *mbox, message_t payload, void *auxPayload = 0)
{
	statusRegister_t s = disableInt();
	message *m = m->newMessage();
	task *receiver = mbox->getReceiver();

	m->payload = payload;
	m->auxPayload = auxPayload;
	m->theTask = receiver;
	m->next = 0;
	mbox->addMessage(m);

	// Put the receiver back onto the run list
	if (mbox->getWaitingHere())
	{
		wakeTask(receiver);

		if (receiver->priority > currentTask->priority)
		{
			if (isrLevel)
				switchPending = 1;
			else
				nextTask();		// Switch to the newly queued task
		}
	}
	restoreInt(s);
	return m;
}

message_t getMessage(mailBox *mbox = &currentTask->defaultMailBox,
							void **pAuxPayload = 0)
{
	message *m;
	message_t ret;
	message *oldm;
	statusRegister_t s = disableInt();

	m = mbox->getHead();

	if (!m)
	{	// No mail available: switch the caller out
		idleTask(currentTask);
		mbox->setWaitingHere(1);
		nextTask();
		mbox->setWaitingHere(0);
	}

	// We're running again: must have mail
	m = mbox->getHead();
	if (!m)
		PANIC();

	ret = m->payload;
	if (pAuxPayload)
		*pAuxPayload = m->auxPayload;

	timer *tp = m->theTimer;
	mbox->deleteHead();
	if (tp)
	{	// Timer expiry message: free up the timer struct for this message
		tp->deleteTimer();
	}
	restoreInt(s);
	return ret;
}

task *createTask(void (*entry)(stackArgument_t, ...),
				task *ptask,
				short runItNow,
				stackArgument_t nargs, ...)
{
	statusRegister_t s = disableInt();
	int argno;
	stackArgument_t *pargs;
	int lnargs = nargs;

	// Set up the argument list for entry to the task.
	lnargs++;						// We also want to stack 'nargs'
	pargs = &nargs + nargs;			// Point beyond last argument
	while (lnargs--)					// Stack all the args
		*(--(ptask->sp.pArguments)) = *--pargs;

	// Set up the entry for the 'rts'
	*(--(ptask->sp.pReturnAddress)) = entry;

	// Set up the initial status register
	*(--(ptask->sp.pStatusRegister)) = initialStatusRegister;

	if (priList == 0)
	{	// This is the first task to start: special processing
		priList = ptask->pPriorityList;
		priList->theTask = ptask;
		priList->next = priList->prev = 0;
		ptask->next = ptask->prev = ptask;
		// The base task is ready to run: normally it won't be started...
		if (runItNow)
			nextTask();
	}
	else
	{
		wakeTask(ptask);
		if (runItNow && ptask->priority > currentTask->priority)
			nextTask();
	}
	restoreInt(s);
	return ptask;
}

static void baseTaskCode(void)
{
	for ( ; ; )
		;
}

task baseTask(nTaskPriorities - 1, "BASETASK");

void startKernel(void)
{
	createTask(baseTaskCode, &baseTask, 0, 0);
}

void task::setPriority(int pri)
{
	priority = pri;
	pPriorityList = &priListPool[pri];
}

task::task(int pri, const char * const name)
{
	setPriority(pri);
	inBoxHead = inBoxTail = 0;
	waitingForMessage = 0;
	onRunList = 0;
	taskName = name;
	sp.pStatusRegister = (statusRegister_t *)&stack[sizeof(stack)];
}

