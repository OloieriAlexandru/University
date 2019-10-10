#pragma once
#include "container.h"
#include "helper.h"
#include <iostream>

class list : public container
{
private:
	struct nodeList
	{
		int value;
		nodeList *nxt;
		nodeList ()
		{
			value = 0;
			nxt = 0;
		}
		nodeList (int x)
		{
			value = x;
			nxt = 0;
		}
	};
	nodeList *root;
	int cnt;
public:
	list ();

	~list ();

	bool Add (int value);

	bool Del (int value);

	int Count (int value);

	bool Exists (int value);

	int* GetSortedArray ();

	int GetCount ();

	friend std::ostream& operator <<(std::ostream &out, const list &l);
};