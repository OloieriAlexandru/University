#pragma once
#include "container.h"

class bst : public container
{
private:
	struct nodeBst
	{
		int value;
		int cnt;
		nodeBst *lft, *rgh;
		nodeBst ()
		{
			value = 0;
			lft = 0;
			rgh = 0;
			cnt = 0;
		}
		nodeBst (int x)
		{
			value = x;
			rgh = 0;
			lft = 0;
			cnt = 1;
		}
	};
	nodeBst *root;
	int cnt;
	
	int countValue (nodeBst *root, int value);

	bool existsValue (nodeBst *root, int value);

	void srd (nodeBst *root, int &act, int *v);

	bool removeValue (nodeBst *root, int value);

	void deleteAllNodes (nodeBst *root);

public:
	bst ();

	~bst ();

	bool Add (int value);

	bool Del (int value);

	int Count (int value);

	bool Exists (int value);

	int* GetSortedArray ();

	int GetCount ();
};