#include "Coleg.h"

Coleg::Coleg (const char * name, const char * phoneNumber, const char * company, const char * address)
{
	Name = (std::string)(name);
	PhoneNumber = (std::string)(phoneNumber);
	Company = (std::string)(company);
	Address = (std::string)(address);
}

Coleg::Coleg (std::string name, std::string phoneNumber, std::string company, std::string address)
{
	Name = name;
	PhoneNumber = phoneNumber;
	Company = company;
	Address = address;
}

std::string Coleg::GetName ()
{
	return Name;
}
