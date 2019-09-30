#pragma once
#include <iostream>

class myClass
{
private:
	int x, y;
	int w;
public:
	myClass (int ix, int iy, int iw) :x (ix), y (iy), w (iw) {};

	int getX ();
	int getY ();
	int getW ();

	friend std::ostream& operator <<(std::ostream &out, const myClass &mC);
};