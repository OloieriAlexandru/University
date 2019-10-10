#pragma once
#include <string>
#include <iostream>
#include "Afisaj.h"

class Anunt : public Afisaj
{
private:
	std::string mesaj;
public:
	Anunt (std::string msg);

	// Inherited via Afisaj
	virtual void Display () override;
};