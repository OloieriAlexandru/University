#pragma once
#include "Advertisment.h"
#include <iostream>

class HouseAdvertisment : public Advertisment
{
private:
	int nrFloors;
public:
	HouseAdvertisment (int s, int p, int nrf);

	void SetNrFloors (int nr);

	// Inherited via Advertisment
	virtual void PrintInfo () override;
};