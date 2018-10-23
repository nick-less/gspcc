#ifndef DD_H_INCLUDED
#define DD_H_INCLUDED

#include <cblock.h>
#include <task.h>

class idriver : public clist
{
	short icanon;		// If non-zero, perform canonical processing
	task *listener;		// Task which wants signalling
	message_t payload;	// Message to send with signal.
public:
	void receivedByte(unsigned char ch)
	{
		int c = ch;
		put(c);
		if (listener && (!icanon || c == '\n'))
			sendMessage(listener, payload);
	}
	void installListener(task *l, message_t p)
	{
		listener = l;
		payload = p;
	}
	void setIcanon(short i)				{ icanon = i; }
	int get(void)						{ return clist::get(); }
};

class odriver : public clist
{
	short ocanon;		// If non-zero, perform canonical processing
	virtual void isrSendByte(unsigned char c);
	virtual void stopTxInts(void);
public:
	// ISR calls this when h/w is ready for a byte
	void sendByte(void)
	{
		if (clistNonEmpty())
			isrSendByte(get());
		else
			stopTxInts();
	}
	void put(int c)
	{
		if (ocanon && c == '\n')
			clist::put('\r');
		clist::put(c);
	}
	void put(unsigned char *p, int len)
	{
		clist::put(p, len);
	}
	void setOcanon(short o)				{ ocanon = o; }
};

#endif		/* DD_H_INCLUDED */

