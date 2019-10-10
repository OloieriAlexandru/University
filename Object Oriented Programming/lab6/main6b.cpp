#include <string>
#include <map>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <utility>
std::map<std::string, int>cnt;
std::vector<std::pair<int, std::string>>ans;
std::string input, currentWord, currentLine;
int main ()
{
	freopen ("input.txt6b", "r", stdin);
	while (std::getline (std::cin, currentLine))
		input = input + currentLine;
	int last = 0, ln = input.size();
	for (int i = 0; i < ln; ++i)
		if (input[i] == '?' || input[i] == '.' || input[i] == '!' || input[i] == ' ')
		{
			currentWord = input.substr (last, i-last);
			++cnt[currentWord];
			last = i + 1;
		}
	for (auto x : cnt)
		ans.push_back (std::make_pair (x.second, x.first));
	sort (ans.begin (), ans.end (), [](std::pair<int, std::string> a, std::pair<int, std::string> b)
	{
		if (a.first == b.first)
		{
			return a.second < b.second;
		}
		return a.first > b.first;
	});
	for (int i = 0; i < std::min (10, (int)ans.size ());++i)
		std::cout << ans[i].second << ' ' << ans[i].first << '\n';
	fclose (stdin);
	return 0;
}