#include <stdio.h>

char _akpmflags[256];
char akpmread = 0;

int akpmflags(int code)
{
	if (!akpmread)
	{
		char *cp;
		FILE *f;

		akpmread = 1;
		cp = getenv("AKPMFLAGS");
		printf("getenv() returns 0x%x\n", cp);
		if (cp)
		{
			f = fopen(cp, "r");
			printf("fopen(%s) returns 0x%x\n", cp, f);
			if (f)
			{
				char buf[100];

				while (fgets(buf, sizeof(buf), f))
				{
					int v;

					v = atoi(buf);
					if (v >= 0 && v < sizeof(_akpmflags))
						_akpmflags[v] = 1;
				}
				fclose(f);
			}
		}
		{
			int i;

			printf("akpmflags:");
			for (i = 0; i < sizeof(_akpmflags); i++)
			{
				if (_akpmflags[i])
					printf(" %d", i);
			}
			printf("\n");
		}
	}
	return _akpmflags[code];
}
