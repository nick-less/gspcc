#ifndef AKPM_SHORT

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __GNUC__
#define inline
#endif

static char SAFavail = 0;
static char *fileNames[20];
static int nFileNames;
static int nShortened;
extern char *xmalloc(int);

#define HTSIZE	1123

struct htab
{
	char *name;
	struct htab *next;
} *htable[HTSIZE];

#define ASIZE	4096

static inline char *xstrdup(char *s)
{
	static int amount = 0;
	static char *buf;
	int len;
	char *ret;

	len = strlen(s) + 1;
	if (len & 1)
		len++;

	if (amount < len)
	{
		buf = xmalloc(ASIZE);
		amount = ASIZE;
	}

	if (amount < len)
	{
		fprintf(stderr, "Panic in shortlabels.c\n");
		exit(1);
	}

	ret = buf;
	buf += len;
	amount -= len;
	return strcpy(ret, s);
}

static inline int iswite(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static inline char *trim(char *s)
{
	char *es;

	while (*s && iswite(*s))
		s++;

	es = s;
	while (*es)
		es++;
	while (es > s && iswite(es[-1]))
	{
		--es;
		*es = 0;
	}
	return s;
}

static inline unsigned hash(char *name)
{
	unsigned ret = 0;

	while (*name)
		ret = ret * 33 + *name++;
	ret %= HTSIZE;
	return ret;
}

static inline void hashadd(char *name)
{
	struct htab **h;

	h = &htable[hash(name)];
	while (*h)
		h = &((*h)->next);
	*h = (struct htab *)malloc(sizeof(struct htab));
	(*h)->name = xstrdup(name);
	(*h)->next = 0;
}

static inline int SAFlookup(char *name)
{
	struct htab *h;
	int ret = 0;

	h = htable[hash(name)];
	while (h)
	{
		if (!strcmp(h->name, name))
		{
			ret = 1;
			break;
		}
		h = h->next;
	}
	return ret;
}

static void readSAF(char *name)
{
	FILE *f;

	f = fopen(name, "r");
	if (!f)
	{
		fprintf(stderr, "gcc: could not open short address file '%s'\n", name);
	}
	else
	{
		char buf[200];
		char *cp;

		while (fgets(buf, sizeof(buf), f))
		{
			char *cp = trim(buf);

			if (*cp == '_')
				cp++;
			if (*cp && *cp != ';' && *cp != '#')
			{
				hashadd(cp);
			}
		}
		SAFavail = 1;
		fclose(f);
	}
}

void stats(void)
{
	gcdb('S', "%d addresses shortened\n", nShortened);
}

int AKPMlookup(char *name)
{
	static char fileread = 0;
	int retval;

	if (!fileread)
	{
		char *fname = getenv("GCC_SHORTADDRESS_FILE");
		int n;

		atexit(stats);

		if (fname)
			readSAF(fname);
		for (n = 0; n < nFileNames; n++)
			readSAF(fileNames[n]);
		fileread = 1;
	}

	retval = SAFavail && SAFlookup(name);
	if (retval)
		nShortened++;
	return retval;
}

void addSAFile(char *name)
{
	if (nFileNames < sizeof(fileNames) / sizeof(*fileNames))
	{
		fileNames[nFileNames++] = strdup(name);
	}
}

#endif /* AKPM_SHORT */
