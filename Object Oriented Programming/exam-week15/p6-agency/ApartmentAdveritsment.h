#pragma once
#include "Advertisment.h"
#include <iostream>

class ApartmentAdvertisment : public Advertisment
{
private:
	int atLevel;
public:
	ApartmentAdvertisment (int s, int p, int l);

	void SetLevel (int nr);

	// Inherited via Advertisment
	virtual void PrintInfo () override;
};