#pragma once
#include "Visitor.h"

class AccVisitor
{
public:
	virtual double Accept (Visitor&) = 0;
protected:
	AccVisitor ();
};