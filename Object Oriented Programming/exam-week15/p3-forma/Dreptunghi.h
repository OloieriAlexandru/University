#pragma once
#include "Forma.h"
#include <iostream>

class Dreptunghi : public Forma
{
protected:
	int x;
	int y;
	int lungime;
	int latime;
public:
	void set (int x, int y, int lungime, int latime);

	// Inherited via Forma
	virtual void Paint () override;
};
