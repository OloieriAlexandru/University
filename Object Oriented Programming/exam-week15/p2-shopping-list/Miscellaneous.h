#pragma once
#include "Item.h"
#include <string>

class Miscellaneous : public Item
{
private:
	int count;
public:
	void setCount (int count);

	// Inherited via Item
	virtual std::string getInfo () override;
};