#pragma once
#include <iostream>

class myObject
{
private:
	int x, y;
public:
	myObject (){}
	myObject (int x, int y) :x (x), y (y) {}
	myObject (const myObject &obj)
	{
		x = obj.x;
		y = obj.y;
	}
	void set (int x, int y);
	myObject& operator ^=(const myObject &a);
	friend bool operator >(const myObject &a, const myObject &b);
	friend std::ostream& operator <<(std::ostream &out, const myObject &b);
};