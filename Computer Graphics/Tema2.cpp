#include <GL/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>

// #include "glut.h" - Olo

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1()
{
	double xmax, ymax, xmin, ymin;
	double a = 1, b = 2;
	double pi = 4 * atan(1);
	double ratia = 0.05;

	// calculul valorilor maxime/minime ptr. x si y
	// aceste valori vor fi folosite ulterior la scalare
	xmax = a - b - 1;
	xmin = a + b + 1;
	ymax = ymin = 0;
	for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia)
	{
		double x1, y1, x2, y2;
		x1 = a + b * cos(t);
		xmax = (xmax < x1) ? x1 : xmax;
		xmin = (xmin > x1) ? x1 : xmin;

		x2 = a - b * cos(t);
		xmax = (xmax < x2) ? x2 : xmax;
		xmin = (xmin > x2) ? x2 : xmin;

		y1 = a * tan(t) + b * sin(t);
		ymax = (ymax < y1) ? y1 : ymax;
		ymin = (ymin > y1) ? y1 : ymin;

		y2 = a * tan(t) - b * sin(t);
		ymax = (ymax < y2) ? y2 : ymax;
		ymin = (ymin > y2) ? y2 : ymin;
	}

	xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
	ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia)
	{
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia)
	{
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x2, y2);
	}
	glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2()
{
	double pi = 4 * atan(1);
	double xmax = 8 * pi;
	double ymax = exp(1.1);
	double ratia = 0.05;

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double x = 0; x < xmax; x += ratia)
	{
		double x1, y1;
		x1 = x / xmax;
		y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();
}

void Display3()
{
	// f(x) = {
	//      1, x = 0
	//      d(x)/x, 0 < x <= 100
	// }

	const int numberOfPoints = 200;
	const double translationValueY = 0.05;
	const double translationValueX = 0.15;
	double xLeft = 0.0;
	double xRight = 100;
	double yMax = 1.0, yMin = 1.0; // din definitia functiei
	double ratio = (xRight - xLeft) / (1.0 * numberOfPoints);
	for (double i = xLeft + ratio; i <= xRight; i += ratio)
	{
		double y = std::abs(i - round(i)) / i;
		yMax = std::max(yMax, y);
		yMin = std::min(yMin, y);
	}

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	glVertex2f(0 - translationValueX, 1.0 - translationValueY);
	for (double i = xLeft + ratio; i <= xRight; i += ratio)
	{
		double x = (1.0 * i) / xRight - translationValueX;
		double y = (std::abs(i - round(i)) / i) / yMax - translationValueY;
		glVertex2f(x, y);
	}
	glEnd();
}


const double PI = 3.14159265359;

void Display4()
{
	// Melcul lui Pascal
	// x = 2 * (a * cos t + b) * cos t
	// y = 2 * (a * cos t + b) * sin t
	// t \in (-pi, pi)

	const int numberOfPoints = 1000;
	const double resizeFactorX = 1.1;
	const double resizeFactorY = 1.5;
	double a = 0.3;
	double b = 0.2;
	double xMin = DBL_MAX;
	double xMax = DBL_MIN;
	double yMin = xMin;
	double yMax = xMax;
	double ratio = (2 * PI) / numberOfPoints;

	for (double i = -PI + ratio; i < PI; i += ratio)
	{
		double x = 2 * (a * cos(i) + b) * cos(i);
		double y = 2 * (a * cos(i) + b) * sin(i);
		xMin = std::min(xMin, x), xMax = std::max(xMax, x);
		yMin = std::min(yMin, y), yMax = std::max(yMax, y);
	}

	xMax = (fabs(xMax) > fabs(xMin)) ? fabs(xMax) : fabs(xMin);
	yMax = (fabs(yMax) > fabs(yMin)) ? fabs(yMax) : fabs(yMin);
	xMax *= resizeFactorX, yMax *= resizeFactorY;

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_LOOP);
	for (double i = -PI + ratio; i < PI; i += ratio)
	{
		double x = 2 * (a * cos(i) + b) * cos(i) / xMax;
		double y = 2 * (a * cos(i) + b) * sin(i) / yMax;
		glVertex2f(x, y);
	}
	glEnd();
}

void Display5()
{
	// Trisectoarea lui Longchamps
	// x = a / (4 * cos^2(t) - 3)
	// y = (a * tan(t)) / (4 * cos^2(t) - 3)
	// t \in (-pi/2, pi/2) \ {(+/-)pi/6}

	const int numberOfPoints = 150;
	const double from = -PI / 2;
	const double to = -PI / 6;
	double a = 0.2;
	double ratio = (to - from) / numberOfPoints;

	const double translationValueY = 0.1;
	const double translationValueX = 0.25;
	glColor3f(0, 0.1, 1.0); // albastru
	glLineWidth(3);
	glBegin(GL_LINE_LOOP);
	std::vector<double> xValues;
	std::vector<double> yValues;

	glVertex2f(-1.0 + translationValueX, 1.0 - translationValueY);

	for (double i = from + ratio; i < to; i += ratio)
	{
		double c = cos(i) * cos(i);
		double x = std::max(a / (4.0 * c - 3.0), -1.0) + translationValueX;
		double y = std::min((a * tan(i)) / (4.0 * c - 3.0), 1.0) - translationValueY;
		glVertex2f(x, y);
		xValues.push_back(x);
		yValues.push_back(y);
	}
	glEnd();
	glLineWidth(1);
	int n = xValues.size();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.1, 0.1); // rosu
	for (int i = 1; i < n - 1; i += 2)
	{
		glVertex2f(xValues[i], yValues[i]);
		glVertex2f(xValues[i + 1], yValues[i + 1]);
		glVertex2f(-1.0 + translationValueX, 1.0 - translationValueY);
	}
	glEnd();
}

void Display6()
{
	// Cicloida
	// x = a * t - b * sin(t)
	// y = a - b * cos(t)
	// t \in R

	const int numberOfPoints = 400;
	const double from = -10.0;
	const double to = 10.0;
	const double resizeFactorX = 1.1;
	const double resizeFactorY = 2.4;
	double a = 0.1;
	double b = 0.2;
	double xMin = DBL_MAX;
	double xMax = DBL_MIN;
	double yMin = xMin;
	double yMax = xMax;
	double ratio = (to - from) / numberOfPoints;

	for (double i = from; i <= to; i += ratio)
	{
		double x = a * i - b * sin(i);
		double y = a - b * cos(i);
		xMin = std::min(xMin, x), xMax = std::max(xMax, x);
		yMin = std::min(yMin, y), yMax = std::max(yMax, y);
	}

	xMax = (fabs(xMax) > fabs(xMin)) ? fabs(xMax) : fabs(xMin);
	yMax = (fabs(yMax) > fabs(yMin)) ? fabs(yMax) : fabs(yMin);
	xMax *= resizeFactorX, yMax *= resizeFactorY;

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double i = from; i <= to; i += ratio)
	{
		double x = (a * i - b * sin(i)) / xMax;
		double y = (a - b * cos(i)) / yMax;
		glVertex2f(x, y);
	}
	glEnd();
}

void Display7()
{
	// Epicicloida
	// x = (R + r) * cos(r/R * t) - r * cos(t + r/R * t)
	// y = (R + r) * sin(r/R * t) - r * sin(t + r/R * t)
	// t \in [0, 2 * pi]

	const int numberOfPoints = 400;
	const double from = 0;
	const double to = 2 * PI;
	const double resizeFactorX = 1.1;
	const double resizeFactorY = 1.3;
	double R = 0.1;
	double r = 0.3;
	double xMin = DBL_MAX;
	double xMax = DBL_MIN;
	double yMin = xMin;
	double yMax = xMax;
	double ratio = (to - from) / numberOfPoints;

	double rR = (1.0 * r) / R;
	for (double i = from; i <= to; i += ratio)
	{
		double x = (R + r) * cos(rR * i) - r * cos(i + rR * i);
		double y = (R + r) * sin(rR * i) - r * sin(i + rR * i);
		xMin = std::min(xMin, x), xMax = std::max(xMax, x);
		yMin = std::min(yMin, y), yMax = std::max(yMax, y);
	}

	xMax = (fabs(xMax) > fabs(xMin)) ? fabs(xMax) : fabs(xMin);
	yMax = (fabs(yMax) > fabs(yMin)) ? fabs(yMax) : fabs(yMin);
	xMax *= resizeFactorX, yMax *= resizeFactorY;

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double i = from; i <= to; i += ratio)
	{
		double x = ((R + r) * cos(rR * i) - r * cos(i + rR * i)) / xMax;
		double y = ((R + r) * sin(rR * i) - r * sin(i + rR * i)) / yMax;
		glVertex2f(x, y);
	}
	glEnd();
}

void Display8()
{
	// Hipocicloida
	// x = (R - r) * cos(r/R * t) - r * cos(t - r/R * t)
	// y = (R - r) * sin(r/R * t) - r * sin(t - r/R * t)
	// t \in [0, 2 * pi]

	const int numberOfPoints = 400;
	const double from = 0;
	const double to = 2 * PI;
	const double resizeFactorX = 1.3;
	const double resizeFactorY = 1.3;
	double R = 0.1;
	double r = 0.3;
	double xMin = DBL_MAX;
	double xMax = DBL_MIN;
	double yMin = xMin;
	double yMax = xMax;
	double ratio = (to - from) / numberOfPoints;

	double rR = (1.0 * r) / R;
	for (double i = from; i <= to; i += ratio)
	{
		double x = (R + r) * cos(rR * i) - r * cos(i - rR * i);
		double y = (R + r) * sin(rR * i) - r * sin(i - rR * i);
		xMin = std::min(xMin, x), xMax = std::max(xMax, x);
		yMin = std::min(yMin, y), yMax = std::max(yMax, y);
	}

	xMax = (fabs(xMax) > fabs(xMin)) ? fabs(xMax) : fabs(xMin);
	yMax = (fabs(yMax) > fabs(yMin)) ? fabs(yMax) : fabs(yMin);
	xMax *= resizeFactorX, yMax *= resizeFactorY;

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double i = from; i <= to; i += ratio)
	{
		double x = ((R - r) * cos(rR * i) - r * cos(i - rR * i)) / xMax;
		double y = ((R - r) * sin(rR * i) - r * sin(i - rR * i)) / yMax;
		glVertex2f(x, y);
	}
	glEnd();
}

void Display9()
{
	// Lemniscata lui Bernoulli
	// r = (+/-)a * sqrt(2 * cos(2t))
	// t \in (-pi/4, pi/4)

	const int numberOfPoints = 400;
	const double from = -PI / 4.0;
	const double to = PI / 4.0;
	const double resizeFactorX = 1.6;
	const double resizeFactorY = 3.0;
	double a = 0.4;
	double xMin = DBL_MAX;
	double xMax = DBL_MIN;
	double yMin = xMin;
	double yMax = xMax;
	double ratio = (to - from) / numberOfPoints;

	for (double i = from + ratio; i < to; i += ratio)
	{
		double r1 = a * sqrt(2 * cos(2 * i));
		double r2 = -r1;
		double x1 = r1 * cos(i), x2 = r2 * cos(i);
		double y1 = r1 * sin(i), y2 = r2 * sin(i);
		xMin = std::min(xMin, std::min(x1, x2)), xMax = std::max(xMax, std::max(x1, x2));
		yMin = std::min(yMin, std::min(y1, y2)), yMax = std::max(yMax, std::max(y1, y2));
	}

	xMax = (fabs(xMax) > fabs(xMin)) ? fabs(xMax) : fabs(xMin);
	yMax = (fabs(yMax) > fabs(yMin)) ? fabs(yMax) : fabs(yMin);
	xMax *= resizeFactorX, yMax *= resizeFactorY;

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	double firstX = 0.0, firstY = 0.0;
	glVertex2f(firstX, firstY);
	for (double i = from + ratio; i < to; i += ratio)
	{
		double r = -1.0 * (a * sqrt(2 * cos(2.0 * i)));

		double x = (r * cos(i)) / xMax;
		double y = (r * sin(i)) / yMax;
		/*if (abs(i - (from + ratio)) < 1e-8)
		{
			firstX = x;
			firstY = y;
		}*/

		glVertex2f(x, y);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex2f(firstX, firstY);
	for (double i = from + ratio; i < to; i += ratio)
	{
		double r = a * sqrt(2 * cos(2.0 * i));

		double x = (r * cos(i)) / xMax;
		double y = (r * sin(i)) / yMax;

		glVertex2f(x, y);
	}
	glEnd();
}

void Display10()
{
	// Spirala logaritmica
	// r = a * e^(1 + t)
	// t \in (0, +inf)

	const int numberOfPoints = 300;
	const double from = 0;
	const double to = 20.0;
	double a = 0.02;
	double ratio = (to - from) / numberOfPoints;

	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double i = from + ratio; i < to; i += ratio)
	{
		double r = a * exp(1.0 + i);
		double x = r * cos(i);
		double y = r * sin(i);

		glVertex2f(x, y);
	}
	glEnd();
}

void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	// glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey)
	{
	case '1':
		Display1();
		break;
	case '2':
		Display2();
		break;
	case '3':
		Display3();
		break;
	case '4':
		Display4();
		break;
	case '5':
		Display5();
		break;
	case '6':
		Display6();
		break;
	case '7':
		Display7();
		break;
	case '8':
		Display8();
		break;
	case '9':
		Display9();
		break;
	case '0':
		Display10();
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}
