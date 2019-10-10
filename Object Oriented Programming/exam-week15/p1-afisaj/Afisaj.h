#pragma once
#include <vector>

class Afisaj
{
protected:
	std::vector<Afisaj*>childs;
public:
	virtual void Display () = 0;
	void Add (Afisaj *af);
};