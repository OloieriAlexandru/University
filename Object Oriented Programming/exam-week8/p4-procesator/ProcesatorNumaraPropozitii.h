#pragma once
#include "Procesator.h"
#include <string>
#include <cstring>
#include <iostream>

class ProcesatorNumaraPropozitii : public Procesator
{
private:
	int cnt;
public:
	ProcesatorNumaraPropozitii ();
	virtual void Proceseaza (std::string str);
	virtual void Proceseaza (const char * str);
	virtual void Print ();
};