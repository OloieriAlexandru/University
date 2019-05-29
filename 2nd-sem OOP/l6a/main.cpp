#include <string>
#include <set>
#include <iostream>
#include <stdio.h>
#include <algorithm>
std::set<int> ans;
std::string input;
int convertToInt (std::string s)
{
	int ln = std::min ((int)s.length (), 9);
	int ret = 0;
	int sign = s[0] == '-' ? -1 : 1;
	for (int i = (s[0] == '-'? 1 : 0); i < ln; ++i)
		ret = ret * 10 + s[i] - '0';
	return ret*sign;
}
int main ()
{
	freopen ("input.txt", "r", stdin);
	std::string delimiter = ",";
	std::getline (std::cin, input);
	input.push_back (',');
	size_t p = 0;
	std::string currentNumber;
	while ((p = input.find (delimiter)) != std::string::npos)
	{
		currentNumber = input.substr (0, p);
		ans.insert (convertToInt (currentNumber));
		input.erase (0, p + delimiter.size ());
	}
	for (auto x : ans)
		std::cout << x << ' ';
	std::cout << '\n';
	fclose (stdin);
	return 0;
}