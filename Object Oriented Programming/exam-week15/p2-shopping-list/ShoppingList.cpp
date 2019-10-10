#include "ShoppingList.h"

void ShoppingList::addItem (Item * it)
{
	items.push_back (it);
}

void ShoppingList::printList ()
{
	for (int i=0;i<items.size();++i)
	{
		std::cout << items[i]->getName () << " : " << items[i]->getInfo () << (i == items.size () - 1 ? "\n" : "; ");
	}
}
