#include <stdio.h>
#include <stdlib.h>
#include "te.h"

class integer
{
	LIST_DECLARE(integer);
	FREELIST_DECLARE(integer);
private:
	int value_;
public:
	integer(int v)
	{
		value_ = v;
	}
	int value(void)
	{
		return value_;
	}
};

FREELIST_DEFINE(integer, 100);

int get(void)
{
	int v;
	char buf[100];

	if (!fgets(buf, 100, stdin))
		v = -1;
	else
		v = atoi(buf);
	return v;
}

void put(int v)
{
	printf(" %d", v);;
}

main()
{
	int v;
	list<integer> intlist;

	while ((v = get()) != -1)
	{
		integer *i = new integer(v);

		intlist.append(i);
	}

	listIter<integer> iter(intlist);
	integer *i = 0;
	int count = 0;

	printf("Pass 1. space=%d, %d items\n", i->nFree(), intlist.nItems());
	while (i = iter())
	{
		if (count++ == 3)
			intlist.insertBefore(i, new integer(33));
		put(i->value());
	}
	printf("\n");

	printf("Pass 2. space=%d, %d items\n", i->nFree(), intlist.nItems());
	iter.reset();
	count = 0;
	while (i = iter())
	{
		put(i->value());
	}
	printf("\n");

	printf("Pass 3. space=%d, %d items\n", i->nFree(), intlist.nItems());
	iter.reset(-1);
	count = 0;
	while (i = iter())
	{
		put(i->value());
	}
	printf("\n");

	exit(0);
}
