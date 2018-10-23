#ifndef SCC_H_INCLUDED
#define SCC_H_INCLUDED

#include <dd.h>

extern "C" void sccISR(void);

class scc : public idriver, public odriver
{
	volatile byteRead_t *dataReadPort;
	volatile byteRead_t *controlReadPort;
	volatile byteWrite_t *dataWritePort;
	volatile byteWrite_t *controlWritePort;

	friend void sccISR(void);

	void wrctl(unsigned short reg, unsigned short val)
	{
		*controlWritePort = reg;
		*controlWritePort = val;
	}
	unsigned char rdctl(unsigned short reg)
	{
		if (reg)
			*controlWritePort = reg;
		return *controlReadPort;
	}
	unsigned char rddata(void)			{ return *dataReadPort; }
	void isrSendByte(unsigned char c)	{ *dataWritePort = c; }
	void stopTxInts(void)				{ wrctl(1, 1); }
public:
	scc(void *datap, void *contp)
	{
		dataReadPort = (volatile byteRead_t *)datap;
		controlReadPort = (volatile byteRead_t *)contp;
		dataWritePort = (volatile byteWrite_t *)datap;
		controlWritePort = (volatile byteWrite_t *)contp;
	}
	void put(int c)						{ odriver::put(c); }
	void put(unsigned char *p, int len)	{ odriver::put(p, len); }
	int get(void)						{ return idriver::get(); }
};

#endif		/* SCC_H_INCLUDED */

