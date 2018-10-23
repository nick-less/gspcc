
extern void panic(char *, int);
#define PANIC()	panic(__FILE__, __LINE__)

