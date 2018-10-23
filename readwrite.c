#ifdef MPW
#include <lib1616.h>

char ismpw;
char mpwread;
extern char *getenv();
char emitNewLines;

readmpw()
{
	if (!mpwread)
	{
		mpwread = 1;
		ismpw = (getenv("MPW") != 0);
	}
	return ismpw;
}

int
read(int fd, void *buf, long nbytes)
{
	int retval;
	char r = '\r';
	char n = '\n';

	if (!mpwread)
		readmpw();

	retval = SSread(fd, buf, nbytes);
	if (retval > 0 && ismpw)
	{
		char *cp = buf;
		char *ecp = cp + retval;

		while (cp < ecp)
		{
			if (*cp++ == r)
				cp[-1] = n;
		}
	}
	return retval;
}

int
write(int fd, char *buf, long nbytes)
{
	int retval;
	char n = '\n';
	char r = '\r';

	if (!mpwread)
		readmpw();

	if (ismpw)
	{
		char *cp = buf;
		char *ecp = cp + nbytes;

		while (cp < ecp)
		{
			if (*cp++ == n)
				cp[-1] = r;
		}
	}
	retval = SSwrite(fd, buf, nbytes);
	return retval;
}
#endif
