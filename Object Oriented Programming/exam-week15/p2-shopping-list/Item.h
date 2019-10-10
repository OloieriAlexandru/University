#pragma once
#include <string>

class Item
{
private:
	std::string name;
public:
	std::string getName ();
	void setName (std::string name);

	virtual std::string getInfo () = 0;
};