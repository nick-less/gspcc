#ifndef MM_H_INCLUDED
#define MM_H_INCLUDED

#include <freelist.h>
#include <list.h>
#include <fileandline.h>
#include <panic.h>
#include <debugflags.h>
#include <task.h>
#include <messages.h>

#define M_MM_MEMUNLOCKED		MESSAGE(MBASE_MM, 1)
#define M_MM_GETMEM				MESSAGE(MBASE_MM, 2)
#define M_MM_FREEMEM			MESSAGE(MBASE_MM, 3)
#define M_MM_GETMEMRESPONSE		MESSAGE(MBASE_MM, 4)
#define M_MM_FREEMEMRESPONSE	MESSAGE(MBASE_MM, 5)

class mem
{
	FREELIST_DECLARE(mem);
	LIST_DECLARE(mem);
private:
	char *addr_;				// Address of this block
	char *fileAndLine_;			// Source file and line of the allocator
	unsigned long length_;		// Length of this block
	unsigned long gap_;			// Amount of free memory after this block
	short locked_;				// If true, this block is inaccessible
	void waitForUnlock(short);	// Wait for this block's freedom
	mailBox *lockBox_;			// Where the waiter waits

	mailBox *lockBox(void)		{ return lockBox_; }
	void setLockBox(mailBox *mbox)
	{
		lockBox_ = mbox;
	}
	unsigned long setGap(unsigned long gap)
	{
		return gap_ = gap;
	}
	unsigned long alterGap(unsigned long incr)
	{
		return gap_ += incr;
	}
	void lock(void)
	{
		if (DBMM && locked_)
			PANIC();
		locked_ = 1;
	}
	void unlock(void)
	{
		if (DBMM && !locked_)
			PANIC();
		locked_ = 0;
	}
public:
	char *addr(void)
	{
		if (locked_)
			waitForUnlock();
		return addr_;
	}
	unsigned long length(void)	{ return length_; }
	unsigned long gap(void)		{ return gap_; }
	char *fileAndLine(void)		{ return fileAndLine_; }
	short locked(void)			{ return locked_; }
	mem(char *addr, unsigned long length, unsigned long gap, char *fileAndLine)
	{
		addr_ = addr;
		length_ = length;
		gap_ = gap;
		fileAndLine_ = fileAndLine;
		locked_ = 0;
		lockBox_ = 0;
	}
	void dumpme(void);
};

extern mem *getMem(unsigned long nbytes, char *fileAndLine = "NONE");
class task;
extern task mmTask;

#define GETMEM(nbytes)	getMem(nbytes, FILEANDLINE())

#endif		/* MM_H_INCLUDED */

