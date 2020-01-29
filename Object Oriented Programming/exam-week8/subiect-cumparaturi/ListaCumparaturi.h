#pragma once
#include "Cumparaturi.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>

class ListaCumparaturi
{
private:
	std::vector<Cumparaturi>lst;
public:
	void Print ();

	friend ListaCumparaturi& operator+=(ListaCumparaturi &l, Cumparaturi c);
	friend ListaCumparaturi& operator-=(ListaCumparaturi &l, std::string t);
	friend ListaCumparaturi operator|(ListaCumparaturi &l1, ListaCumparaturi &l2);
};
