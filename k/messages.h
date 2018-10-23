#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

typedef int message_t;

#define MBASE_TIMER		1

#define MESSAGE(BASE, MSG)		(((BASE) << 8) | (MSG))

#endif		/* MESSAGES_H_INCLUDED */

