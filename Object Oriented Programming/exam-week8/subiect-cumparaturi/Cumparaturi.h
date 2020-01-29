#pragma once
#include <string>
#include <iostream>

class Cumparaturi
{
private:
	std::string type;
	int cnt;
public:
	Cumparaturi (std::string t, int c);

	friend std::ostream& operator <<(std::ostream &out, const Cumparaturi &c);

	std::string GetType ();

	int GetCnt ();
	void AddCnt (int amount);
};