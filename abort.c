#ifdef MPW
/* Abort function */

#include <stdio.h>
#include <string.h>

extern start();
extern int _Htext;

#define OK(a)	(((long)(a) > (long)start) && ((long)(a) < (long)(&_Htext)))
#define OKL(n)	OK(__builtin_return_address(n))

extern char *input_filename;
extern int lineno;

void abort(void)
{
	unsigned long reta = __builtin_return_address(0);
	unsigned long s = (unsigned long)start;

	fprintf(stderr, "Abort!!!!  input_filename='%s', lineno=%d\n", input_filename, lineno);
	fprintf(stderr, "Called from 0x%x\n", reta);
	fprintf(stderr, "start()=0x%x, offset=0x%x\n", s, reta - s);
	fflush(stderr);
	fflush(stdout);

	if (OKL(0))
	{
		fprintf(stderr, "Called from 0x%x\n", __builtin_return_address(1) - s);
		if (OKL(1))
		{
			fprintf(stderr, "Called from 0x%x\n", __builtin_return_address(2) - s);
			if (OKL(2))
			{
				fprintf(stderr, "Called from 0x%x\n", __builtin_return_address(3) - s);
				if (OKL(3))
				{
					fprintf(stderr, "Called from 0x%x\n", __builtin_return_address(4) - s);
					if (OKL(4))
					{
						fprintf(stderr, "Called from 0x%x\n", __builtin_return_address(5) - s);
						if (OKL(5))
						{
							fprintf(stderr, "Called from 0x%x\n", __builtin_return_address(6) - s);
							if (OKL(6))
							{
								fprintf(stderr, "Called from 0x%x\n",
									__builtin_return_address(7) - s);
								if (OKL(7))
								{
									fprintf(stderr, "Called from 0x%x\n",
										__builtin_return_address(8) - s);
									if (OKL(8))
									{
										fprintf(stderr, "Called from 0x%x\n",
											__builtin_return_address(9) - s);
										if (OKL(9))
										{
											fprintf(stderr, "Called from 0x%x\n",
												__builtin_return_address(10) - s);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	exit(1);
}
#else

#include <stdio.h>

abort()
{
	fprintf(stderr, "abort!!!!!\n");
	exit(1);
}
#endif
