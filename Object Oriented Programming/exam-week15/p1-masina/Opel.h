#pragma once
#include "MasinaOras.h"
#include <string>

class Opel : public MasinaOras
{
private:
	int capacitate;
	std::string culoare;
	int anFabricatie;
public:
	void setCapacitate (int cap);
	void setCuloare (std::string cul);
	void setAnFabricatie (int an);
	int getAnFabricatie ();

	// Inherited via MasinaOras
	virtual int getCapacitate () override;
	virtual std::string getCuloare () override;

	// Inherited via MasinaOras
	virtual std::string getName () override;
};