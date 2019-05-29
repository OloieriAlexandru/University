#pragma once
#include "Visitor.h"
#include "Lamaie.h"
#include "Portocala.h"
#include "Grepfruit.h"
#include <iostream>

class VitaminCVisitor : public Visitor
{
public:
	VitaminCVisitor ();
	// Inherited via Visitor
	virtual double VisitLamaie (Lamaie * l) override;
	virtual double VisitPortocala (Portocala * p) override;
	virtual double VisitGrepfruit (Grepfruit * g) override;
};
