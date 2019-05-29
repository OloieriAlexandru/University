#pragma once
#include <iostream>
#include <utility>

class myClass
{
private:
	int value1;
	int value2;
public:
	myClass () {}
	myClass (int v1, int v2);
	myClass (std::pair<int, int> v);
	void set (int v1, int v2);
	bool operator >(const myClass &obj);
	friend std::ostream& operator <<(std::ostream &out, const myClass &obj);
};