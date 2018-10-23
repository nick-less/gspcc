template <class T> class freeList
{
	static freeList<T> *list;
	freeList<T> *next;
	T *t;

public:
	T *get(void)
	{
		T *ret = list->t;
		list = list->next;
		return ret;
	}
	void put(T *at)
	{
		next = list;
		list = this;
	}
	freeList(void)
	{
		next = list;
		list = this;
	}
};

class thing
{
public:
	int a;
};

freeList<thing> things[20];

#include <stdio.h>

main()
{
	int i;
	thing *save[20];

	for (i = 0; i < 10; i++)
	{
		thing *t;

		t = things[0].get();
		t->a = i;
		save[i] = t;
	}

	for (i = 0; i < 10; i++)
	{
		save[i]->put();
	}

	for (i = 0; i < 10; i++)
	{
		freeList<thing> *t;

		t = things.get();
		printf("%d\n", t->a);
	}
}

