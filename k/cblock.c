#include <cblock.h>
#include <macros.h>

static cblock cblockPool[cblockPoolSize];

void cblock::initCblocks(void)
{
	int i;

	cblockPool[NELEM(cblockPool) - 1].next = 0;
	for (i = 0; i < NELEM(cblockPool) - 1; i++)
	{
		cblockPool[i].next = &cblockPool[i + 1];
	}
	freeList = &cblockPool[0];
}

