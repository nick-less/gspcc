#include <task.h>
#include <mm.h>
#include <panic.h>

FREELIST_DEFINE(mem, 100);

typedef list<mem>		memList;
typedef listIter<mem>	memIter;

memList mL;					// The main list

char *arena;				// Start of malloc arena
unsigned long arenaSize;	// Number of byets at *arena
char *arenaEnd;				// arena + arensSize
#define MSIZE	1000
char _arena[MSIZE];			// debug

static void mmDump(void);

class memReq
{
	mailBox mbox_;
	unsigned long nbytes_;
	mem *obtainedMem_;
	mem *freedMem_;
	char *fileANdLine_;
	enum { getmem, freemem } reqType_;
public:
	memReq(unsigned long nbytes, char *fileAndLine)
	{
		nbytes_ = nbytes;
		fileAndLine_ = fileAndLine;
		reqType_ = getmem;
	}
	memReq(mem *m)
	{
		freedMem_ = m;
		reqType = freemem;
	}
	mailBox *mbox(void)				{ return &mbox_; }
	unsigned long nbytes(void)		{ return nbytes_; }
	mem *obtainedMem(void)			{ return obtainedMem_; }
	mem *freedMem(void)				{ return freedMem_; }
	char *fileAndLine(void)			{ return fileAndLine_; }
};

static void fail1(char *msg, mem *m)
{
	eprintf("mmValidate() failure: %s\n", msg);
	eprintf("Offending mem: ");
	m->dumpme();
	mmDump();
	PANIC();
}

static void fail2(char *msg, mem *m1, mem *m2)
{
	eprintf("mmValidate() failure [2]: %s\n", msg);
	eprintf("Offending mem1: ");
	m1->dumpme();
	eprintf("Offending mem2: ");
	m2->dumpme();
	mmDump();
	PANIC();
}

static void mmValidate(void)
{
	mmIter iter(mL);
	mem *probe = 0;
	mem *probeNext;

	probeNext = iter();
	ASSERT(probeNext == mL.first());
	while (probeNext)
	{
		probe = probeNext;
		probeNext = iter();
		if (probe->addr() < arena)
			fail1("bad addr", probe);
		if (probe->addr() + probe->length() > arenaEnd)
			fail1("bad addr+length", probe);
		if (probe->addr() + probe->length() + probe->gap() > arenaEnd)
			fail1("bad addr+length+gap", probe);
		if (probe->addr()+probe->length()+probe->gap() != probeNext->addr())
			fail2("don't add up", probe, probeNext);
	}
	if (probe->addr() != arenaEnd)
		fail1("lost arenaEnd", probe);
	if (probe->length())
		fail1("last probe length", probe);
	if (probe->gap())
		fail1("last probe gap", probe);
}

// Allocate some memory.
// Use best fit algorithm.
static mem *MMgetMem(unsigned long nbytes, char *fileAndLine)
{
	nbytes = (nbytes + 3) & ~3;

	if (DBMM)
		mmValidate();

	// Scan the list looking for the best fitter.
	mem *ret;
	memIter iter(mL);
	mem *mLLast = mL.last();
	mem *probe;
	mem *best = 0;
	unsigned long bestGap = 0xffffffff;

	while ((probe = iter()) != mLLast)
	{
		ASSERT(probe);
		unsigned long thisGap = probe->gap();
		if (thisGap >= nbytes && thisGap < bestGap)
		{
			best = probe;
			bestGap = thisGap;
		}
	}
	if (best == 0)
	{
		eprintf("getMem(%d): will not fit\n", nbytes);
		PANIC();
	}
	// We can fit this block after *best
	ret = best->insertAfter(new mem(	best->addr() + best->length(),
										nbytes,
										nbytes - bestGap,
										fileAndLine));
	best.setGap(0);
	if (DBMM)
		mmValidate();
	return ret;
}

static void MMfreeMem(mem *theMem)
{	// Add the size of this gap to the predecessor
	if (DBMM)
		mmValidate();
	theMem->getPrev().alterGap(theMem->gap());
	delete mL.remove(theMem);
	if (DBMM)
		mmValidate();
}

#if 0
// Background process to defragment the MM tables.
// We scan the list until we find a gap.  We then
// scan from that point looking for the largest mem
// which can be moved into that gap.
static void mmSweep(void)
{
	int didOne = 0;
	mem *mLLast = mL.last();

	if (DBMM)
		mmValidate();

	while (!didOne)
	{
		mem *probe;
		mem *bestFit = 0;
		memIter iter(mL);

		// Search the list, not looking at the last entry, which
		// is the zero length tail holder
		while (!didOne && (probe = iter()) != mLLast)
		{
			ASSERT(probe);
			unsigned long gap = probe->gap();
			unsigned long bestFitLength = 0;
			memIter searchIter(probe);	// Another iterator, placed at *probe
			mem *search;

			searchIter();					// Skip to next mem
			while ((search = searchIter()) != mLLast)
			{
				ASSERT(search);
				unsigned long searchLength = search->length();
				if (searchLength <= gap && searchLength > bestFitLength)
				{
					bestFit = search;
					bestFitLength = searchLength;
				}
			}
			if (bestFit)
			{	// OK, now move the mem at *bestFit to be immediately
				// after *probe
				if (DBMM)
				{
					ASSERT(bestFit->length() <= probe->gap());
					ASSERT(	(	probe->addr() +
								probe->length() +
								bestFit->length()
							) <=
							(
								probe->getNext()->addr
							));
				}
				// Copy the data
				memcpy(	probe->addr() + probe->length(),
						bestFit->addr(),
						bestFit->length());

				// Adjust the gap size of the predecessor of the move source
				// This predecessor may in fact be 'probe'.
				bestFit->getPrev()->alterGap(bestFit->length());

				// Remove the source mem from the mm list
				mL.remove(bestFit);

				// Set the gap size of the move dest
				bestFit->setGap(probe->gap() - bestFit->length());

				// Set the gap size of the move dest's predecessor (probe)
				probe->setGap(0);

				// Put the move dest into the list
				ml.insertAfter(probe, bestFit);

				didOne = 1;
			}
		}
	}
	if (DBMM)
		mmValidate();
}
#endif

// Another background process to defragment the MM tables.
// We just sit there sweeping the table, moving blocks forward.
// If moving a block will result in temporary invalidation
// of its contents, we lock it during the move.
static int mmSweep2(void)
{
	mem *mLLast = mL.last();
	mem *probe, *probePrev;
	memIter iter(mL);
	inr didOne = 0;

	if (DBMM)
		mmValidate();

	probePrev = iter();
	probe = iter();
	while (!didOne && probe != mLLast)
	{
		ASSERT(probe);
		if (probePrev->gap())
		{	// Move *probe forwards
			int locked;
			if (probe->length() < probePrev->gap())
			{	// *probe will be invalidated during the move
				probe->lock();
				locked = 1;
			}
			else
			{
				locked = 0;
			}
			memmove(probePrev->addr() + probePrev->length(),
					probe->addr(),
					probe->length();

			// probe's gap has now been merger with prevProbe's
			probe->alterGap(probePrev->gap());

			// probePrev's gap has vanished
			probePrev->setGap(0);

			// probe has moved: assume that the following write
			// is atomic, so we don't need to lock anything
			probe->setAddr(probePrev->addr() + probePrev->length());

			if (locked)
			{
				probe->unlock();
				if (probe->lockBox())
				{	// Wake up the sleeper
					sendMessage(probe->lockBox(), M_MM_MEMUNLOCKED);
				}
			}
			didOne = 1;
		}
		else
		{
			probePrev = probe;
			probe = iter();
		}
	}

	if (DBMM)
		mmValidate();

	return didOne;
}

void mem::dumpme(void)
{
	eprintf("mem at 0x%x: %s addr=0x%x[%d], length=%d, gap=%d\n",
			(long)this,
			fileAndLine(),
			(long)addr(),
			addr() - arena,
			length(),
			gap());
}

static void mmDump(void)
{
	memIter iter(mL);
	mem *m = 0;

	eprintf("Memory manager tables:\n");
	eprintf("Freelist holds %d mems\n", m->freeListNFree());
	m->theRatchet.dumpme();
	while ((m = iter()))
	{
		m->dumpme();
	}
}

static void mmInit(void)
{
	arena = _arena;
	arenaSize = sizeof(_arena);
	arenaEnd = arena + arenaSize;

	// Create the bounds of the MM table

	// List head:  addr = arena, gap = arenaSize
	ml.append(new mem(arena, 0, arenaSize, FILEANDLINE()));

	// List tail:  addr = arena + arenaSize, gap = 0
	ml.append(new mem(arenaEnd, 0, 0, FILEANDLINE());
}

//
// A mem is under reconstruction, but someone wants the pointer
// to its contents.  Put the caller to sleep until the reconstruction
// is complete.
//
void mem::waitForUnlock(void)
{
	mailBox mbox;

	if (DBMM)
	{
		eprintf("%s awaits mem: ", taskName());
		dumpme();
	}
	setLockBox(&mbox);
	ASSERT(getMessage(&mbox) == M_MM_MEMUNLOCKED);
	setLockBox(0);
	if (DBMM)
	{
		eprintf("%s runs again.  mem: ", taskName());
		dumpme();
	}
}

static void mmCode(void);
task mmTask(2, "MM", mmCode);

// Entry point: allocate some memory
mem *getMem(unsigned long nbytes, char *fileAndLine = "NONE")
{
	memReq mreq(nbytes, fileAndLine);

	sendMessage(mmTask, M_MM_GETMEM, &mreq);
	ASSERT(getMessage(mreq.mbox()) == M_MM_GETMEMRESPONSE);
	return mreq.obtainedMem();
}

// Entry point: free some memory
void freeMem(mem *m)
{
	memReq mreq(m);

	sendMessage(mmTask, M_MM_FREEMEM, &mreq);
	ASSERT(getMessage(mreq.mbox()) == M_MM_FREEMEMRESPONSE);
}

#if 0
//
// The memory allocation server
//
static void mmCode(void)
{
	printf("MM starts\n");

	mmInit();
	mmValidate();

	for ( ; ; )
	{
		if (messageReady())
		{
			memReq *mreq;

			message_t msg = getMessage(&mreq);
			switch (msg)
			{
			case M_MM_GETMEM:
				mreq->obtainedMem_ =
					MMgetMem(mreq->nbytes(), mreq->fileAndLine());
				sendMessage(mreq->mbox, M_MM_GETMEMRESPONSE);
				break;
			case M_MM_FREEMEM:
				MMfreeMem(mreq->freedMem());
				break;
			default:
				PANIC();
			}
		}
		else
		{
			mmSweep2();
		}
	}
}

// Test code

static void mmCode(void)
{
	printf("MM starts\n");
	mmInit();

	mem *m1, *m2, *m3, *m4;

	m1 = getMem(100);
	mmDump();
	m2 = getMem(200);
	mmDump();
	m3 = getMem(300);
	mmDump();
	m4 = getMem(400);
	mmDump();
	freeMem(m3);
	mmDump();
	m3 = getMem(200);
	mmDump();
}

Check that the freelist code copes with panicing if
	a: free things are returned
	b: non-free things are obtained.
