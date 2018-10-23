struct thing
{
	int a;
	int b;
} array[32];

foo1(p1, p2)
{
	struct thing *p;

	p = &array[p1 + 2];

	if (p->b)
		boo();
}

foo2(p1)
{
	struct thing *p;

	p = &array[10];

	if (p->b)
		boo();
}

foo3(int p1, struct thing *p)
{
	p[p1 + 12].b = 18;
}

	
