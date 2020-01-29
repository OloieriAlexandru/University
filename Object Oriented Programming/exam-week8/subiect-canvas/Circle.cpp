#pragma once
#include "Circle.h"

void Circle::Paint (Canvas * c, int v1, int v2, int v3 , int v4 ,int v5)
{
	int n = c->GetHeight ();
	int m = c->GetWidth ();
	char st = (char)v4;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			double a = i;
			double b = j;
			double d = sqrt ((a - v2)*(a - v2) + (b - v1)*(b - v1));
			double dreq = (double)v3;
			if (d <= dreq)
			{
				c->operator()(i, j) = st;
			}
		}
	}
}
