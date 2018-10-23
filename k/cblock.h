#ifndef CBLOCK_H_INCLUDED
#define CBLOCK_H_INCLUDED

#include <interrupts.h>
#include <panic.h>
#include <debugflags.h>
#include <stdlib.h>

static const int cblockBufSize = 64;
static const int cblockPoolSize = 256;

class cblock
{
	static cblock *freeList;
	static short initialised;

	cblock *next;
	int writeIndex;
	int readIndex;
	unsigned char buf[cblockBufSize];

	friend class clist;
	void initCblocks(void);

protected:
	cblock *newCblock(void)
	{
		cblock *ret = freeList;
		freeList = ret->next;
		return ret;
	}

	void deleteCblock(void)
	{
		next = freeList;
		freeList = this;
	}

	void initialise(void)
	{
		if (!initialised)
			initCblocks();
	}
};

class clist
{
	cblock *readCblock;
	cblock *writeCblock;

	cblock *growList(void)
	{
		cblock *cb = cb->newCblock();
		writeCblock->next = cb;
		writeCblock = cb;
		cb->next = 0;
		cb->readIndex = 0;
		cb->writeIndex = 0;
		return cb;
	}

	cblock *shrinkList(void)
	{
		cblock *cb, *cb2;

		cb = readCblock;
		readCblock = cb2 = cb->next;
		cb->deleteCblock();
		return cb2;
	}

protected:
	clist(void)
	{
		readCblock->initialise();
		readCblock = writeCblock = readCblock->newCblock();
		readCblock->writeIndex = readCblock->readIndex = 0;
	}

	void put(int c)
	{
		cblock *cb = writeCblock;
		if (cb->writeIndex == cblockBufSize)
			cb = growList();
		cb->buf[cb->writeIndex++] = c;
	}

	void put(unsigned char *p, int len)
	{
		cblock *cb = writeCblock;
		int ntocopy;

		while (len)
		{
			if (cb->writeIndex == cblockBufSize)
				cb = growList();
			ntocopy = cblockBufSize - cb->writeIndex;
			if (ntocopy > len)
				ntocopy = len;
			memcpy(cb->buf + cb->writeIndex, p, ntocopy);
			len -= ntocopy;
			cb->writeIndex += ntocopy;
			p += ntocopy;
		}
	}
 
	int get(void)
	{
		cblock *cb = readCblock;

		if (DBcblock)
		{
			if (cb == 0)
				PANIC();
			if (cb == writeCblock && cb->readIndex == cb->writeIndex)
				PANIC();
		}
		if (cb->readIndex == cblockBufSize)
			cb = shrinkList();
		return cb->buf[cb->readIndex++];
	}

	int clistNonEmpty(void)
	{
		if (readCblock == writeCblock)
			return readCblock->writeIndex - readCblock->readIndex;
		return 1;
	}
};

#endif		/* CBLOCK_H_INCLUDED */

