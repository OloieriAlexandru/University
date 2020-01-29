#include <iostream>
#include "Rectangle.h"
#include "Circle.h"
#include "Canvas.h"

using namespace std;

int main ()
{
	//Canvas c; // ACEST COD NU VA COMPILA
	Canvas c (20, 20);
	c.AddPainter ("cerc", new Circle ());
	c.AddPainter ("rect", new Rectangle ());
	c (0, 0) = 'C'; c (1, 0) = '+'; c (2, 0) = '+';
	c.Paint ("cerc", 5, 7, 3, 'x'); // cerc x=5,y=7, raza=3, fill='x'
	c.Paint ("rect", 10, 10, 18, 14, 'd'); // dreptunghi (10,10) - (18,4) - fill='d'
	c.Paint ("rect", 0, 19, 19, 19, '-');
	c.Paint ("rect", 19, 0, 19, 19, '|');
	c (19, 19) = '+';
	for (int y = 0; y < c.GetHeight (); y++) {
		for (int x = 0; x < c.GetWidth (); x++)
			cout << c (x, y);
		cout << endl;
	}
	return 0;
}