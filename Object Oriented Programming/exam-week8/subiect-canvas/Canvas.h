#pragma once
#include <vector>
#include <string>
#include <map>
#include "IPainter.h"

class Canvas
{
private:
	std::vector<std::vector<char>>matrix;
	std::map<std::string, IPainter*>mp;
public:
	Canvas (int lines, int cols);

	int GetWidth ();
	int GetHeight ();

	void AddPainter (std::string painter, IPainter *x);
	void AddPainter (const char *painter, IPainter *x);

	void Paint (std::string s, int x1, int y1, int x2, int y2, char c);
	void Paint (const char * st, int x1, int y1, int x2, int y2, char c);

	void Paint (std::string s, int x, int y, int r, char c);
	void Paint (const char *st, int x, int y, int r, char c);

	char& operator()(int x, int y);
};