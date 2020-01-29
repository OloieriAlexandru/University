#include "Canvas.h"

Canvas::Canvas (int lines, int cols)
{
	matrix.resize (lines);
	for (int i = 0; i < lines; ++i)
	{
		matrix[i].resize (cols, ' ');
	}
}

int Canvas::GetWidth ()
{
	return matrix[0].size ();
}

int Canvas::GetHeight ()
{
	return matrix.size ();
}

void Canvas::AddPainter (std::string painter, IPainter * x)
{
	mp[painter] = x;
}

void Canvas::AddPainter (const char * painter, IPainter *x)
{
	std::string s (painter);
	AddPainter (s, x);
}

void Canvas::Paint (std::string s, int x1, int y1, int x2, int y2, char c)
{
	if (!mp.count (s))
	{
		return;
	}
	mp[s]->Paint (this, x1, y1, x2, y2, (int)c);
}

void Canvas::Paint (const char * st, int x1, int y1, int x2, int y2, char c)
{
	std::string s (st);
	Paint (s, x1, y1, x2, y2, c);
}

void Canvas::Paint (std::string s, int x, int y, int r, char c)
{
	if (!mp.count(s))
	{
		return;
	}
	mp[s]->Paint (this, x, y, r, (int)c);
}

void Canvas::Paint (const char * st, int x, int y, int r, char c)
{
	std::string s (st);
	Paint (s, x, y, r, c);
}

char & Canvas::operator()(int x, int y)
{
	char nimic = '\0';
	if (x<0 || y<0 || x>(int)matrix.size () || y>(int)matrix[0].size ())
	{
		return nimic;
	}
	return matrix[x][y];
}
