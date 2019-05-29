#pragma once
#include <string>

template <class TNode>
class node
{
public:
	node *next;
	std::string key;
	TNode info;
	node (std::string hash, TNode value)
	{
		next = 0;
		key = hash;
		info = value;
	}
};