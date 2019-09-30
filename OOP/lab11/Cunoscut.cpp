#include "Cunoscut.h"

Cunoscut::Cunoscut (const char * name, const char * phoneNumber)
{
	Name = (std::string)(name);
	PhoneNumber = (std::string)(phoneNumber);
}

Cunoscut::Cunoscut (std::string name, std::string phoneNumber)
{
	Name = name;
	PhoneNumber = phoneNumber;
}

std::string Cunoscut::GetName ()
{
	return Name;
}
