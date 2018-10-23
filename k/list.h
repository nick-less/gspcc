#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

template<class T> class list
{
private:
	T *first_, *last_;
	int nItems_;
public:
	inline list()
	{
		first_ = 0;
		last_ = 0;
		nItems_ = 0;
	};
	inline T *first(void)
	{
		return first_;
	}
	inline T *last(void)
	{
		return last_;
	}
	inline T *append(T *t)
	{
		if (!first_)
		{
			first_ = t;
			last_ = t;
			t->setPrev(0);
			t->setNext(0);
		}
		else
		{
			last_->setNext(t);
			t->setNext(0);
			t->setPrev(last_);
			last_ = t;
		}
		nItems_++;
		return t;
	};
	inline T *prepend(T *t)
	{
		if (!first_)
		{
			append(t);
		}
		else
		{
			t->setNext(first_);
			first_->setPrev(t);
			t->setPrev(0);
			first_ = t;
		}
		nItems_++;
		return t;
	};
	inline T *remove(T *t)
	{
		if (first_ == last_)
		{
			first_ = 0;
			last_ = 0;
		}
		else if (t == first_)
		{
			t->setPrev(0);
			first_ = t->getNext();
			first_->setPrev(0);
		}
		else if (t == last_)
		{
			last_ = t->getPrev();
			last_->setNext(0);
		}
		else
		{
			t->getPrev()->setNext(t->getNext());
			t->getNext()->setPrev(t->getPrev());
		}
		nItems_--;
		return t;
	}
	inline T *insertAfter(T *pos, T *t)
	{
		if (pos == last_)
		{
			append(t);
		}
		else
		{
			t->setNext(pos->getNext());
			t->setPrev(pos);
			pos->setNext(t);
			t->getNext()->setPrev(t);
		}
		return t;
	}
	inline T *insertBefore(T *pos, T *t)
	{
		if (pos == first_)
		{
			prepend(t);
		}
		else
		{
			insertAfter(pos->getPrev(), t);
		}
		return t;
	}
	inline int nItems(void)
	{
		return nItems_;
	}
};

template<class T> class listIter
{
private:
	T *here;
	list<T> *theList;
	int direction_;
public:
	inline void reset(int direction = 1)
	{
		direction_ = direction;
		here = (direction > 0) ? theList->first() : theList->last();
	}
	inline listIter(list<T> &l, int direction = 1)
	{
		theList = &l;
		reset(direction);
	};
	inline listIter(listIter<T> &li, int direction = 1)
	{
		theList = li.theList;
		reset(direction);
		here = li.here;
	};

	inline T *operator ()(void)
	{
		T *ret = here;
		if (here)
			here = (direction_ > 0) ? here->getNext() : here->getPrev();
		return ret;
	};
};

#define LIST_DECLARE(CLASS)							\
private:											\
	CLASS *next, *prev;								\
public:												\
	inline CLASS *getNext(void)		{ return next; }\
	inline void setNext(CLASS *n)	{ next = n; }	\
	inline CLASS *getPrev(void)		{ return prev; }\
	inline void setPrev(CLASS *n)	{ prev = n; }

#endif		/* LIST_H_INCLUDED */

