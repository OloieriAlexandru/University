#pragma once
#include "Dreptunghi.h"

class DreptunghiRotit : public Dreptunghi
{
private:
	int unghi;
public:
	void set (int x, int y, int lungime, int latime, int unghi);

	virtual void Paint () override;
};