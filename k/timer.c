#include <timer.h>
#include <panic.h>
#include <messages.h>
#include <machine.h>

extern task *currentTask;

tick_t currentTick;

static timer *pendingTimers;
timer *timer::freeList = 0;

timer *setTimer(task *receiver, tick_t when)
{
	statusRegister_t s = disableInt();
	timer *ret = ret->newTimer();
	timer *tp;

	ret->expiryTime = when;

	// Sort this timer into the global list
	tp = pendingTimers;
	if (tp == 0)
	{
		pendingTimers = ret;
		ret->next = 0;
		ret->prev = 0;
	}
	else if (when < tp->expiryTime)
	{	// Insert at head
		ret->prev = 0;
		ret->next = tp;
		pendingTimers = ret;
	}
	else
	{
		timer *tp2;

		while ((tp2 = tp->next) && tp2->expiryTime < when)
			tp = tp->next;
		// Insert after *tp
		ret->next = tp2;
		ret->prev = tp;
		if (tp2)
			tp2->prev = ret;
		tp->next = ret;
	}
	tp->where = timer::delivered;
	tp->receiver = receiver;
	restoreInt(s);
	return tp;
}

void killTimer(timer *tp)
{
	statusRegister_t s = disableInt();

	switch (tp->where)
	{
	case timer::pending:
		if (tp->prev == 0)
		{	// At head of list
			pendingTimers = tp->next;
			tp->next->prev = 0;
		}
		else
		{
			tp->prev->next = tp->next;
			if (tp->next)
				tp->next->prev = tp->prev;
		}
		tp->deleteTimer();
		break;
	case timer::delivered:
		// Remove the message from the task's inbox
		message *mp = tp->createdMessage;
		if (mp->prev == 0)
		{
			task *ptask = mp->theTask;
			ptask->inBoxHead = mp->next;
			ptask->inBoxHead->prev = 0;
		}
		else
		{
			mp->prev->next = mp->next;
			if (mp->next)
				mp->next->prev = mp->prev;
		}
		tp->deleteTimer();
		mp->deleteMessage();
		break;
	case timer::freelist:
		PANIC();
		break;
	default:
		PANIC();
	}
	restoreInt(s);
}

extern "C" void timerISR(void)
{
	timer *tp;
	message *m;

	clearHardwareTimer();

	currentTick++;
	tp = pendingTimers;
	while (tp && tp->expiryTime <= currentTick)
	{
		m = sendMessage(tp->receiver, M_TIMEREXPIRY);
		m->theTimer = tp;
		tp->createdMessage = m;
		tp->where = timer::delivered;
	}
}
