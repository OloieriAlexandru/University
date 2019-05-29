#include "bst.h"

int bst::countValue (nodeBst * root, int value)
{
	if (!root)
	{
		return 0;
	}
	if (root->value == value)
	{
		return root->cnt;
	}
	int returnedValue = countValue (root->lft, value);
	if (returnedValue)
	{
		return returnedValue;
	}
	returnedValue = countValue (root->rgh, value);
	if (returnedValue)
	{
		return returnedValue;
	}
	return 0;
}

bool bst::existsValue (nodeBst * root, int value)
{
	if (!root)
	{
		return false;
	}
	int ans = false;
	ans = ans || existsValue (root->lft, value);
	ans = ans || existsValue (root->rgh, value);
	if (root->value == value && root->cnt)
	{
		ans = true;
	}
	return ans;
}

void bst::srd (nodeBst * root, int & act, int *v)
{
	if (!root)
	{
		return;
	}
	srd (root->lft, act, v);
	for (int i = 0; i < root->cnt; ++i)
	{
		v[act++] = root->value;
	}
	srd (root->rgh, act, v);
}

// o manareala 
bool bst::removeValue (nodeBst * root, int value)
{
	if (!root)
	{
		return false;
	}
	if (root->value == value)
	{
		if (root->cnt)
		{
			--root->cnt;
			return true;
		}
		return false;
	}
	bool ans = false;
	ans = ans || removeValue (root->lft, value);
	ans = ans || removeValue (root->rgh, value);
	return ans;
}

void bst::deleteAllNodes (nodeBst * root)
{
	if (!root)
	{
		return;
	}
	deleteAllNodes (root->lft);
	deleteAllNodes (root->rgh);
	delete root;
}

bst::bst ()
{
	root = 0;
	cnt = 0;
}

bst::~bst ()
{
	deleteAllNodes (root);
}

bool bst::Add (int value)
{
	++cnt;
	nodeBst *newNode = new nodeBst (value);
	if (!root)
	{
		root = newNode;
		return true;
	}
	nodeBst *p = root;
	nodeBst *pred = 0;
	while (p)
	{
		pred = p;
		if (p->value > value)
		{
			p = p->lft;
		}
		else if (p->value < value)
		{
			p = p->rgh;
		}
		else
		{
			++p->cnt;
			p = 0;
		}
	}
	if (pred->value != value)
	{
		if (pred->value > value)
		{
			pred->lft = newNode;
		}
		else
		{
			pred->rgh = newNode;
		}
	}
	return true;
}

bool bst::Del (int value)
{
	bool ret = removeValue (root, value);
	if (ret)
	{
		--cnt;
	}
	return ret;
}

int bst::Count (int value)
{
	return countValue (root, value);
}

bool bst::Exists (int value)
{
	return existsValue (root, value);
}

int * bst::GetSortedArray ()
{
	int *ans = new int[cnt];
	int act = 0;
	srd (root, act, ans);
	return ans;
}

int bst::GetCount ()
{
	return cnt;
}
