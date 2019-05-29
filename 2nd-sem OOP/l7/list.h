#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include "node.h"

template <class TIterL>
class listIterator
{
public:
	node<TIterL> *curr;
	listIterator& operator++()
	{
		curr = curr->next;
		return *this;
	}
	bool operator != (listIterator it)
	{
		return curr != it.curr;
	}
	std::pair<std::string, TIterL> operator* ()
	{
		return std::make_pair (curr->key, curr->info);
	}
};

template <class TList>
class list
{
private:	
	node<TList> *root;
	int count;
	friend class listIterator<TList>;
public:
	list ()
	{
		root = 0, count = 0;
	}
	~list ()
	{
		removeAll ();
	}
	list (const list<TList> &obj)
	{
		count = obj.count;
		root = 0;
		if (!obj.root) return;
		root = new node<TList> (obj.root->info);
		node<TList> *p = obj.root->next;
		node<TList> *last = root;
		while (p)
		{
			last->next = new node<TList> (p->info);
			last = last->next;
			p = p->next;
		}
	}
	int getCount ()
	{
		return count;
	}
	void removeAll ()
	{
		if (!root) return;
		node<TList> *p = root;
		while (p)
		{
			node<TList> *nxt = p->next;
			delete p;
			p = nxt;
		}
	}
	void insert (std::string key, TList e)
	{
		++count;
		node<TList> *neww = new node<TList> (key, e);
		if (!root)
			root = neww;
		else
			neww->next = root, root = neww;
	}
	bool remove (std::string key)
	{
		if (!root) return false;
		if (root->key == key)
		{
			--count;
			node<TList> *first = root;
			root = root->next;
			delete first;
			return true;
		}
		node<TList> *p = root;
		node<TList> *last = 0;
		while (p && p->key == key)
		{
			last = p;
			p = p->next;
		}
		if (!p) return false;
		--count;
		last->next = p->next;
		delete p;
		return true;
	}
	bool exists (std::string key)
	{
		if (!root) return false;
		node<TList> *p = root;
		while (p)
		{
			if (p->key == key)
				return true;
			p = p->next;
		}
		return false;
	}
	void printAll (std::ostream &out)
	{
		if (!root) return;
		node *p = root;
		while (p)
		{
			out << p->info << ' ';
			p = p->next;
		}
	}
	bool empty ()
	{
		return count == 0;
	}
	TList& operator[] (std::string key)
	{
		node<TList> *p = root;
		while (p)
		{
			if (p->key == key)
				return p->info;
			p = p->next;
		}
		TList x;
		memset (&x, 0, sizeof (x));
		insert (key, x);
		return root->info;
	}
	static bool sortCmp (std::pair<std::string, TList> &a, std::pair<std::string, TList> &b)
	{
		if (a.first == b.first)
		{
			return a.second > b.second;
		}
		return a.first > b.first;
	}
	void sort ()
	{
		if (!root) return;
		std::pair<std::string, TList> *arr = new std::pair<std::string, TList>[count];
		node<TList> *p = root;
		for (int i = 0; i < count; ++i, p = p->next)
			arr[i].first = p->key, arr[i].second = p->info;
		std::sort (arr, arr+count, sortCmp);
		std::reverse (arr, arr + count);
		p = root;
		for (int i = 0; i < count; ++i, p = p->next)
			p->info = arr[i].second, p->key = arr[i].first;
		delete[] arr;
	}
	listIterator<TList> begin ()
	{
		listIterator<TList> it;
		it.curr = (!root ? 0 : root);
		return it;
	}
	listIterator<TList> end ()
	{
		listIterator<TList> it;
		it.curr = 0;
		return it;
	}
};