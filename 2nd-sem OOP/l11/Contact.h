#pragma once
#include <string>

class Contact
{
protected:
	std::string Name;
public:
	virtual std::string GetName () = 0;
};