#include <GL/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <assert.h>

// dimensiunea ferestrei in pixeli
#define DIM 300
#define EPS 0.16
#define PI 3.14159265359

unsigned char prevKey;

std::string intToString(int value)
{
	std::string res;
	if (value < 0)
	{
		res.push_back('-');
		value = -value;
	}
	while (value >= 10)
	{
		res.push_back((value % 10) + '0');
		value /= 10;
	}
	res.push_back((value % 10) + '0');
	std::reverse(res.begin(), res.end());
	return res;
}

class CartesianGrid
{
private:
	struct Point
	{
		int line;
		int column;

		Point()
		{
			line = 0;
			column = 0;
		}

		Point(int l, int c)
		{
			line = l;
			column = c;
		}
	};

	int linesCount;
	int colsCount;
	double epsilon;

	double getSide()
	{
		double verticalSide = (2.0 - 2 * epsilon) / linesCount;
		double horizontalSide = (2.0 - 2 * epsilon) / colsCount;
		return std::min(verticalSide, horizontalSide);
	}

	// din tema 1
	void DisplayConvexPolygon(double size, double xCenter, double yCenter, int pointsCount, void (*setColorCall)())
	{
		setColorCall();
		glBegin(GL_POLYGON);
		double angle = 360.0 / pointsCount;
		for (int i = 0; i < pointsCount; ++i)
		{
			double x = xCenter + cos((i * angle) * PI / 180.0) * size;
			double y = yCenter + sin((i * angle) * PI / 180.0) * size;
			glVertex2f(x, y);
		}
		glEnd();
	}

	void setCoordinateXY(int line, int column, double& x, double& y, double side)
	{
		x = -1.0 + epsilon + column * side;
		y = -1.0 + epsilon + line * side;
	}

	void validateCoordinate(int line, int column)
	{
		if (!(line >= 0 && line <= linesCount && column >= 0 && column <= colsCount))
		{
			std::string message = std::string("Invalid line and columns: (") + intToString(line) + ", " +
				intToString(column) + ")!";
			std::cout << message << '\n';
			throw std::invalid_argument(message);
		}
	}

	std::vector<Point> getLinePointsX(int x0, int y0, int xN, int yN, int size)
	{
		std::vector<Point> points;
		double dx = (double)xN - x0;
		double dy = (double)yN - y0;

		int yInc = dy >= 0 ? 1 : -1;
		int xInc = dx >= 0 ? 1 : -1;
		double m = dx != 0 ? (dy / dx) : 0;

		int slopeSign = m >= 0 ? 1 : -1;

		int d = xInc * 2 * dy - yInc * dx;
		int dE = xInc * 2 * dy;
		int dNE = xInc * 2 * dy - yInc * 2 * dx;
		int x = x0;
		int y = y0;

		points.push_back(Point(y, x));
		for (int i = 0; i < size / 2; ++i)
		{
			points.push_back(Point(y - 1, x));
			points.push_back(Point(y + 1, x));
		}
		do
		{
			if (slopeSign * d < 0)
			{
				d += dE;
				x += xInc;
			}
			else
			{
				d += dNE;
				x += xInc;
				y += yInc;
			}
			points.push_back(Point(y, x));
			for (int i = 0; i < size / 2; ++i)
			{
				points.push_back(Point(y - 1, x));
				points.push_back(Point(y + 1, x));
			}
		}
		while (x != xN);
		return points;
	}

	std::vector<Point> getLinePointsY(int x0, int y0, int xN, int yN, int size)
	{
		std::vector<Point> points;
		double dx = (double)xN - x0;
		double dy = (double)yN - y0;

		int yInc = dy >= 0 ? 1 : -1;
		int xInc = dx >= 0 ? 1 : -1;
		double m = (dy / dx);

		int slopeSign = m >= 0 ? 1 : -1;

		int d = xInc * dy - yInc * 2 * dx;
		int dN = - yInc * 2 * dx;
		int dNE = xInc * 2 * dy - yInc * 2 * dx;
		int x = x0;
		int y = y0;

		points.push_back(Point(y, x));
		for (int i = 0; i < size / 2; ++i)
		{
			points.push_back(Point(y, x - 1));
			points.push_back(Point(y, x + 1));
		}
		do
		{
			if (slopeSign * d > 0)
			{
				d += dN;
				y += yInc;
			}
			else
			{
				d += dNE;
				x += xInc;
				y += yInc;
			}
			points.push_back(Point(y, x));
			for (int i = 0; i < size / 2; ++i)
			{
				points.push_back(Point(y, x - 1));
				points.push_back(Point(y, x + 1));
			}
		}
		while (y != yN);
		return points;
	}

public:
	CartesianGrid(int linesCount, int colsCount)
		: linesCount(linesCount), colsCount(colsCount), epsilon(EPS)
	{
	}

	CartesianGrid(int linesCount, int colsCount, double epsilon)
		: linesCount(linesCount), colsCount(colsCount), epsilon(epsilon)
	{
	}

	void displayGrid()
	{
		glColor3f(0.5, 0.5, 0.5); // gri
		double minSide = getSide();

		double xLeft = -1.0 + epsilon;
		double xRight = 1.0 - epsilon;
		double yLines = -1.0 + epsilon;
		for (int i = 0; i <= linesCount; ++i, yLines += minSide)
		{
			glBegin(GL_LINE_STRIP);
			glVertex2f(xLeft, yLines);
			glVertex2f(xRight, yLines);
			glEnd();
		}

		double yTop = 1.0 - epsilon;
		double yDown = -1.0 + epsilon;
		double xLines = -1.0 + epsilon;
		for (int i = 0; i <= colsCount; ++i, xLines += minSide)
		{
			glBegin(GL_LINE_STRIP);
			glVertex2f(xLines, yDown);
			glVertex2f(xLines, yTop);
			glEnd();
		}
	}

	void displayPixel(int line, int column, bool terminate = true)
	{
		try
		{
			validateCoordinate(line, column);
		}
		catch (std::invalid_argument e)
		{
			if (terminate)
			{
				throw;
			}
			else
			{
				return;
			}
		}
		double minSide = getSide();

		double x, y;
		setCoordinateXY(line, column, x, y, minSide);
		double radius = minSide / 3.0;

		glPolygonMode(GL_FRONT, GL_FILL);
		DisplayConvexPolygon(radius, x, y, 20, []()
		{
			glColor3f(0.05, 0.05, 0.05); // negru
		});
	}

	void displayLine(int line1, int column1, int line2, int column2, int size)
	{
		validateCoordinate(line1, column1);
		validateCoordinate(line2, column2);
		double minSide = getSide();

		double x1, y1;
		double x2, y2;
		setCoordinateXY(line1, column1, x1, y1, minSide);
		setCoordinateXY(line2, column2, x2, y2, minSide);

		glColor3f(0.9, 0.1, 0.1); // rosu
		glBegin(GL_LINE_STRIP);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();
		std::vector<Point> points;
		if (column2 == column1 || fabs((line2 - line1) / (column2 - column1)) > 1)
			points = getLinePointsY(column1, line1, column2, line2, size);
		else
			points = getLinePointsX(column1, line1, column2, line2, size);
		for (auto p : points)
		{
			displayPixel(p.line, p.column, false);
		}
	}
};

void Display0()
{
	CartesianGrid grid(15, 15);
	grid.displayGrid();
	grid.displayPixel(3, 3);
	grid.displayPixel(0, 0);
	grid.displayPixel(9, 15);
}

void Display1()
{
	CartesianGrid grid(20, 20);
	grid.displayGrid();
	grid.displayLine(0, 10, 20, 10, 3);
	grid.displayLine(0, 7, 15, 0, 1);
	grid.displayLine(0, 0, 7, 15, 1);
	grid.displayLine(15, 0, 10, 15, 3);
}

void Display2()
{
}

void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey)
	{
	case '0':
		Display0();
		break;
	case '1':
		Display1();
		break;
	case '2':
		Display2();
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

	glutInitWindowSize(DIM, DIM);

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
