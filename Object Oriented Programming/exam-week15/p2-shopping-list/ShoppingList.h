#pragma once
#include "Item.h"
#include <vector>
#include <iostream>

class ShoppingList
{
private:
	std::vector<Item*>items;
public:
	void addItem (Item *it);
	void printList ();
};