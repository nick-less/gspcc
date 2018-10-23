#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED
// Tasks

#include <types.h>
#include <interrupts.h>
#include <messages.h>

static const int nTaskPriorities = 16;
static const int maxTasks = 64;
static const int taskStackSize = 2048;		// Bytes
static const int maxMessages = 256;

class task;
class priorityList;
class timer;
class message;

class message
{
	static message *freeList;
	friend class idriver;
	friend message *sendMessage(task *receiver, message_t payload, void *auxPayload = 0);
	friend message_t getMessage(void **pAuxPayload = 0);
public:
	message *next, *prev;		// Doubly linked so they can be removed
	message_t payload;
	void *auxPayload;
	timer *theTimer;			// The timer struct which caused this message
	task *theTask;				// The task to which it is sent

	message(void)
	{
		next = freeList;
		freeList = this;
	}

	// This method must be called with interrupt protection
	message *newMessage(void)
	{
		message *ret = freeList;
		freeList = ret->next;
		ret->theTimer = 0;
		return ret;
	}

	// These methods must be called with interrupt protection
	void deleteMessage(void)
	{
		this->next = freeList;
		freeList = this;
	}
	void deleteMessage(message *m)		{ m->deleteMessage(); }

};

class mailBox
{
	message *messageHead, *messageTail;
	task *receiver;
	short waitingHere;
public:
	mailBox(task *ptask = currentTask)
	{
		messageHead = 0;
		messageTail = 0;
		waitingHere = 0;
		receiver = ptask;
	}
	void deleteHead(void)
	{
		message *m = messageHead;
		if (!m)
			PANIC();
		messageHead = m->next;
		m->deleteMessage();
		m = messageHead;
		if (!m)
			messageTail = 0;
	}
	message *addMessage(message *m)
	{
		if (inBoxHead == 0)
		{	// The receiver's queue is empty
			messageHead = m;
			messageTail = m;
			m->prev = 0;
		}
		else
		{
			messageTail->next = m;
			m->prev = messageTail;
			messageTail = m;
		}
		return m;
	}

	message *getHead(void)		{ return messageHead; }
	void setWaitingHere(short w){ waitingHere = w; }
	short getWaitingHere(void)	{ return waitingHere; }
	task *getReceiver(void)
	{
		if (receiver == 0)
			PANIC();
		return receiver;
	}
};

extern priorityList priListPool[nTaskPriorities];

class priorityList
{
public:
	priorityList *next, *prev;
	task *theTask;
	short priority;		// The position in priListPool[]

	priorityList(void)
	{
		theTask = 0;
		next = prev = 0;
		priority = this - &priListPool[0];
	}
};

typedef union
{
	returnAddress_t *pReturnAddress;
	statusRegister_t *pStatusRegister;
	stackArgument_t *pArguments;
} sp_t;

class task
{
public:
	task *next, *prev;
	short priority;
	priorityList *pPriorityList;
	mailBox defaultMailBox(this);
	short onRunList;
	sp_t sp;
	const char *taskName;
	char stack[taskStackSize];

	void setPriority(int pri);
	task(int pri, const char * const name);
};

extern "C" void nextTask(void);
task *createTask(void (*entry)(stackArgument_t, ...),
				task *ptask,
				short runItNow,
				stackArgument_t nargs, ...);
void startKernel(void);

#endif		/* TASK_H_INCLUDED */

