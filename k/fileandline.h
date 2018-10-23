#ifndef FILEANDLINE_H_INCLUDED
#define FILEANDLINE_H_INCLUDED

/*
 * Wierd hack to return current file and line as a string constant "file:line"
 */

#define STRINGIFYLINE__(n)	#n
#define LINEASSTRING__(n)	STRINGIFYLINE__(n)
#define FILEANDLINE()		__FILE__ ":" LINEASSTRING__(__LINE__)

#endif		/* FILEANDLINE_H_INCLUDED */

