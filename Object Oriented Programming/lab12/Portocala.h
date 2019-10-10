#pragma once
#include <string>
#include "Fruct.h"
#include "AccVisitor.h"
#include "Visitor.h"

class Visitor;

class Portocala : public Fruct, AccVisitor
{
public:
	Portocala (int dim, std::string cul);
	// Inherited via AccVisitor
	virtual double Accept (Visitor &v) override;
};