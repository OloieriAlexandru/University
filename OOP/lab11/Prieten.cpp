#include "Prieten.h"

Prieten::Prieten (std::string name, std::string birthDay, std::string phoneNumber, std::string address)
{
	Name = name;
	BirthDay = birthDay;
	PhoneNumber = phoneNumber;
	Address = address;
}

Prieten::Prieten (const char * name, const char * birthDay, const char * phoneNumber, const char * address)
{
	Name = (std::string)(name);
	BirthDay = (std::string)(birthDay);
	PhoneNumber = (std::string)(phoneNumber);
	Address = (std::string)(address);
}

std::string Prieten::GetName ()
{
	return Name;
}

void Prieten::SetName (std::string newName)
{
	Name = newName;
}
