#pragma once
#include "Puncte2d.h"
#include <initializer_list>
#include <iostream>
#include <utility>
#include <tuple>

class Puncte3d : public Puncte2d 
{
private:
	int *z;
public:
	Puncte3d (std::initializer_list < std::tuple<int,int,int>> l);
	~Puncte3d ();

	void SumaAxe () override;
};