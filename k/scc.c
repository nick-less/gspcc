#include <scc.h>

scc scc0((void *)0x123456, (void *)0x234567);
scc scc1((void *)0x1234568, (void *)0x2345678);

extern "C" void sccISR(void)
{
	unsigned scc0ctl0 = scc0.rdctl(0);
	if (scc0ctl0 & 1)
		scc0.receivedByte(scc0.rddata());
	if (scc0ctl0 & 2)
		scc0.sendByte();
}

char buf[100];

void t(int x)
{
	scc0.put(x);
	scc0.put(buf, 30);
}

int bar(void)
{
	return scc0.get();
}
