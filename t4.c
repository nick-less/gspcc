struct ss
{
	int a;
	int b;
};

extern void *malloc(int);
extern void borp(struct ss *);

struct ss add(struct ss ss1, struct ss ss2)
{
	struct ss *ps = (struct ss *)malloc(sizeof(struct ss));

	ps->a = ss1.a + ss2.a;
	return *ps;
}

bop()
{
	struct ss ss1, ss2;
	struct ss result;

	result = add(ss1, ss2);
	borp(&result);
}
