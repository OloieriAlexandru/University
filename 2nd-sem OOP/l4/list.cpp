#include "list.h"

list::list ()
{
	root = 0;
	cnt = 0;
}

list::~list ()
{
	while (root)
	{
		nodeList *nxt = root->nxt;
		delete root;
		root = nxt;
	}
}

bool list::Add (int value)
{
	nodeList *newNode = new nodeList (value);
	if (!root)
	{
		root = newNode;
	}
	else
	{
		nodeList *p = root;
		while (p->nxt)
		{
			p = p->nxt;
		}
		p->nxt = newNode;
	}
	++cnt;
	return true;
}

bool list::Del (int value)
{
	nodeList *p = root;
	if (root->value == value)
	{
		--cnt;
		root = root->nxt;
		delete p;
		return true;
	}
	while (p->nxt)
	{
		if (p->nxt->value == value)
		{
			--cnt;
			nodeList *r = p->nxt;
			p->nxt = p->nxt->nxt;
			delete r;
			return true;
		}
		p = p->nxt;
	}
	return false;
}

int list::Count (int value)
{
	int ret = 0;
	nodeList *p = root;
	while (p)
	{
		if (p->value == value)
		{
			++ret;
		}
		p = p->nxt;
	}
	return ret;
}

bool list::Exists (int value)
{
	nodeList *p = root;
	while (p)
	{
		if (p->value == value)
		{
			return true;
		}
		p = p->nxt;
	}
	return false;
}

int * list::GetSortedArray ()
{
	int *ret = new int[cnt];
	nodeList *p = root;
	for (int i = 0; i < cnt; ++i)
	{
		ret[i] = p->value;
		p = p->nxt;
	}
	helper::sort (ret, cnt);
	return ret;
}

int list::GetCount ()
{
	return cnt;
}

std::ostream & operator<<(std::ostream & out, const list & l)
{
	list::nodeList *p = l.root;
	for (int i=0;i<l.cnt;++i)
	{
		out << p->value << (const char*)(i == l.cnt-1? "  " : "->");
		p = p->nxt;
	}
	out << '\n';
	return out;
}
