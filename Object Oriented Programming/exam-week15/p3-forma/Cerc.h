#pragma once
#include "Forma.h"
#include <iostream>

class Cerc : public Forma
{
private:
	int x;
	int y;
	int raza;
public:
	void set (int x, int y, int raza);

	// Inherited via Forma
	virtual void Paint () override;
};