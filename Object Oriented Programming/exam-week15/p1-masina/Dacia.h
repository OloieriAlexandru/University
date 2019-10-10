#pragma once
#include "MasinaOras.h"
#include <string>

class Dacia : public MasinaOras
{
private:
	int capacitate;
	std::string culoare;
public:
	void setCapacitate (int cap);
	void setCuloare (std::string cul);

	// Inherited via MasinaOras
	virtual int getCapacitate () override;
	virtual std::string getCuloare () override;

	// Inherited via MasinaOras
	virtual std::string getName () override;
};