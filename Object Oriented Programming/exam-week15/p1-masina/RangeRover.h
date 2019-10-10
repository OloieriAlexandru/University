#pragma once
#include "SUV.h"

class RangeRover : public SUV
{
private:
	int consum;
public:
	void setConsum (int con);

	// Inherited via SUV
	virtual int getConsum () override;

	// Inherited via SUV
	virtual std::string getName () override;
};