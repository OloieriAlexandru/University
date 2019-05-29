#pragma once
#include "Contact.h"

class Cunoscut : public Contact
{
private:
	std::string PhoneNumber;
public:
	Cunoscut (const char *name, const char *phoneNumber);
	Cunoscut (std::string name, std::string phoneNumber);

	// Inherited via Contact
	virtual std::string GetName () override;
};