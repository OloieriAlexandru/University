#pragma once
#include "Contact.h"

class Coleg : public Contact
{
private:
	std::string PhoneNumber;
	std::string Company;
	std::string Address;
public:
	Coleg (const char *name, const char *phoneNumber, const char *company, const char *address);
	Coleg (std::string name, std::string phoneNumber, std::string company, std::string address);

	// Inherited via Contact
	virtual std::string GetName () override;

};