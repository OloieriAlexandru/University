#pragma once
#include <string>
#include "Fruct.h"
#include "AccVisitor.h"
#include "Visitor.h"

class Visitor;

class Lamaie : public Fruct, AccVisitor
{
public:
	Lamaie (int dim, std::string cul);
	// Inherited via AccVisitor
	virtual double Accept (Visitor &v) override;
};