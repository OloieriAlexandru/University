#include "ListaCumparaturi.h"

void ListaCumparaturi::Print ()
{
	int sz = lst.size ();
	for (int i = 0; i < sz; ++i)
	{
		std::cout << lst[i];
		if (i != (sz - 1))
		{
			std::cout << " , ";
		}
	}
	std::cout << '\n';
}

ListaCumparaturi & operator+=(ListaCumparaturi & l, Cumparaturi c)
{
	int rm = -1;
	bool found = false;
	for (int i=0;i<l.lst.size();++i)
		if (l.lst[i].GetType () == c.GetType ())
		{
			found = true;
			l.lst[i].AddCnt (c.GetCnt ());
			if (!l.lst[i].GetCnt ())
			{
				rm = i;
				break;
			}
		}
	if (rm != -1)
	{
		l.lst.erase (l.lst.begin () + rm);
	}
	if (!found)
	{
		l.lst.push_back (c);
	}
	return l;
}

ListaCumparaturi & operator-=(ListaCumparaturi & l, std::string t)
{
	int rm = -1;
	for (int i = 0; i < l.lst.size (); ++i)
	{
		if (l.lst[i].GetType() == t)
		{
			rm = i;
			break;
		}
	}
	if (rm != -1)
	{
		l.lst.erase (l.lst.begin () + rm);
	}
	return l;
}

ListaCumparaturi operator|(ListaCumparaturi & l1, ListaCumparaturi & l2)
{
	ListaCumparaturi ans;
	std::map<std::string, int>mp;
	for (auto x : l1.lst)
	{
		mp[x.GetType ()] += x.GetCnt ();
	}
	for (auto x : l2.lst)
	{
		mp[x.GetType ()] += x.GetCnt ();
	}
	for (auto x : mp)
	{
		ans.lst.push_back (Cumparaturi (x.first, x.second));
	}
	return ans;
}
