class base
{
public:
	virtual void virtfunc(void);
	int a;
};

class derived : public base
{
public:
	void virtfunc(void);
};

base b;
