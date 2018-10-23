#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void akpm(int code)
{
	fprintf(stderr, "AKPM: code %d\n", code);
	fflush(stderr);
}

void gcdb(int c, char *fmt, long p1, long p2, long p3, long p4, long p5)
{
	static char readgdb = 0;
	static char *ep;

	if (!readgdb)
	{
		ep = getenv("GCC_DEBUG");
		if (ep)
			ep = (char *)strdup(ep);
		readgdb = 1;
	}

	if (ep && (*ep == '*' || strchr(ep, c)))
	{
		fprintf(stderr, fmt, p1, p2, p3, p4, p5);
	}
}

void AKPMmain(int argc, char *argv[])
{
}

char _flags[256];

void note_flags(char *s)
{
	while (*s)
	{
		_flags[(unsigned)*s] = 1;
		s++;
	}
}

int flags(char c)
{
	return _flags[(unsigned)c];
}	
