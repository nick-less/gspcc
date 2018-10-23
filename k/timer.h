// Timer stuff

#include <types.h>
#include <task.h>
#include <messages.h>

#define M(MSG)			MESSAGE(MBASE_TIMER, MSG)

#define M_TIMEREXPIRY	M(1)

timer *				setTimer(task *receiver, tick_t when);
void				killTimer(timer *pt);
extern "C" void		timerISR(void);

class timer
{
	static timer *freeList;
	enum { freelist, delivered, pending, unknown } where;
	tick_t	expiryTime;		// When it goes off
	timer *next, *prev;		// Links for freelist and timer queue
	task *receiver;			// Who it is sent to
	message *createdMessage;// Where it is on the receiver's inBox
	friend void timerISR(void);
	friend timer *setTimer(task *receiver, tick_t when);
	friend void killTimer(timer *pt);
	friend timer *setTimer(tick_t when);

public:
	timer(void)
	{
		where = freelist;
		next = freeList;
		freeList = this;
	}

	// This method must be called with interrupt protection
	timer *newTimer(void)
	{
		timer *ret = freeList;
		freeList = ret->next;
		ret->where = unknown;
		return ret;
	}

	// These methods must be called with interrupt protection
	void deleteTimer(void)
	{
		where = freelist;
		next = freeList;
		freeList = this;
	}

	void deleteTimer(timer *tp)			{ tp->deleteTimer(); }
};

static inline timer *setTimer(tick_t when)
{
	extern task *currentTask;
	return setTimer(currentTask, when);
}
