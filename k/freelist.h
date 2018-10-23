#ifndef FREELIST_H_INCLUDED
#define FREELIST_H_INCLUDED

typedef long unsigned int size_t;

#define FREELIST_DECLARE(CLASS)						\
private:											\
	static CLASS *freeList;							\
	static int nFree_;								\
public:												\
	inline CLASS()									\
	{												\
		setNext(freeList);							\
		freeList = this;							\
		nFree_++;									\
	}												\
	inline void *operator new(size_t n = 0)			\
	{												\
		CLASS *ret = freeList;						\
		freeList = freeList->getNext();				\
		nFree_--;									\
		return (void *)ret;							\
	}												\
	inline void operator delete(void *x)			\
	{												\
		CLASS *c = (CLASS *)x;						\
		c->setNext(freeList);						\
		freeList = c;								\
		nFree_++;									\
	}												\
	inline int nFree(void)							\
	{												\
		return nFree_;								\
	}

#define FREELIST_DEFINE(CLASS, SIZE)				\
CLASS CLASS##Pool[SIZE];							\
CLASS *CLASS::freeList;								\
int CLASS::nFree_

#endif		/* FREELIST_H_INCLUDED */

