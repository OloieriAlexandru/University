#include "Rectangle.h"

void Rectangle::Paint (Canvas * c, int v1, int v2, int v3, int v4, int v5)
{
	char set = (char)v5;
	for (int i = v1; i <= v3; ++i)
	{
		for (int j = v2; j <= v4; ++j)
		{
			c->operator()(i, j) = set;
		}
	}
}
