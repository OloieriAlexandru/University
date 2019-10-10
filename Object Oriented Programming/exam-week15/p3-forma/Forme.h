#pragma once
#include "Forma.h"
#include <vector>

class Forme : public Forma
{
private:
	std::vector<Forma*>forms;
public:
	void Add (Forma *f);

	// Inherited via Forma
	virtual void Paint () override;
};