#pragma once
#include "Contact.h"

class Prieten : public Contact
{
private:
	std::string BirthDay;
	std::string PhoneNumber;
	std::string Address;
public:
	Prieten (std::string name, std::string birthDay, std::string phoneNumber, std::string address);
	Prieten (const char *name, const char *birthDay, const char *phoneNumber, const char *address);

	// Inherited via Contact
	virtual std::string GetName () override;
	void SetName (std::string newName);
};