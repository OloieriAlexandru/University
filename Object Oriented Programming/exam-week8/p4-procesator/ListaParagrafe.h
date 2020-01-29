#pragma once
#include <vector>
#include <string>

class ListaParagrafe
{
private:
	std::vector<std::string> v;
public:
	void Add (std::string str);
	void Add (const char *str);

	std::vector<std::string>::iterator begin ()
	{
		std::vector<std::string>::iterator it = v.begin ();
		return it;
	}
	std::vector<std::string>::iterator end ()
	{
		std::vector<std::string>::iterator it = v.end ();
		return it;
	}
};