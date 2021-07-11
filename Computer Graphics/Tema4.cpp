#include <GL/freeglut.h>

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <set>

//#include "glut.h" - Olo

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
		int x;
		int y;

		Point() : x(0), y(0)
		{
		}

		Point(int y, int x) : x(x), y(y)
		{
		}
	};

	struct Edge
	{
		Point vi;
		Point vf;

		Edge()
		{
		}

		Edge(Point p1, Point p2)
		{
			vi = p1;
			vf = p2;
		}
	};

	struct Polygon
	{
		std::vector<Edge> edges;

		std::set<std::pair<int, int>> getPoints()
		{
			std::set<std::pair<int, int>> st;
			for (auto e : edges)
			{
				st.insert(std::make_pair(e.vi.x, e.vi.y));
				st.insert(std::make_pair(e.vf.x, e.vf.y));
			}
			return st;
		}

		std::set<std::pair<int, int>> getToDisplayPoints()
		{
			std::set<std::pair<int, int>> st;
			auto handleEdge = [&](const Edge& e)
			{
				if (e.vi.y <= e.vf.y)
				{
					st.insert(std::make_pair(e.vi.x, e.vi.y));
				}
				else
				{
					st.insert(std::make_pair(e.vf.x, e.vf.y));
				}
			};
			for (auto e : edges)
			{
				handleEdge(e);
			}
			return st;
		}
	};

	struct Intersection
	{
		int yMax;
		double xMin;
		double ratio;

		Intersection() : yMax(0), xMin(0), ratio(0)
		{
		}

		Intersection(int yMax, double xMin, double ratio) : yMax(yMax), xMin(xMin), ratio(ratio)
		{
		}
	};

	struct Intersections
	{
		std::vector<Intersection> intersections;
	};

	struct ET
	{
		std::unordered_map<int, Intersections> mp;
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

	void DisplayEllipse(double a, double b, double xCenter, double yCenter, int pointsCount, void (*setColorCall)())
	{
		setColorCall();
		glBegin(GL_POLYGON);
		double angle = 360.0 / pointsCount;
		for (int i = 0; i < pointsCount; ++i)
		{
			double x = xCenter + cos((i * angle) * PI / 180.0) * a;
			double y = yCenter + sin((i * angle) * PI / 180.0) * b;
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

	void polyRead(FILE* fileIn, Polygon& p)
	{
		int n, x, y;
		fscanf_s(fileIn, "%d", &n);
		fscanf_s(fileIn, "%d %d", &x, &y);
		Point initial(y, x);
		Point lastPoint = initial;
		Point point;

		for (int i = 2; i <= n; ++i)
		{
			fscanf_s(fileIn, "%d %d", &x, &y);
			point = Point(y, x);
			p.edges.push_back(Edge(lastPoint, point));
			lastPoint = point;
		}
		p.edges.push_back(Edge(lastPoint, initial));
	}

	void polyDisplay(const Polygon& p)
	{
		double minSide = getSide();
		double x, y;

		glColor3f(1.0, 0.1, 0.1); // rosu
		glLineWidth(3.0f);
		glBegin(GL_LINE_LOOP);
		for (auto& edge : p.edges)
		{
			x = -1.0 + minSide * edge.vi.x + epsilon;
			y = -1.0 + minSide * edge.vi.y + epsilon;
			glVertex2f(x, y);
		}
		glEnd();
		glLineWidth(1.0f);
	}

	void polyInitET(Polygon& p, ET& et)
	{
		int xm, ym;
		int xM, yM;

		for (auto& e : p.edges)
		{
			if (e.vi.y != e.vf.y)
			{
				ym = std::min(e.vi.y, e.vf.y);
				yM = std::max(e.vi.y, e.vf.y);
				xm = (ym == e.vi.y) ? e.vi.x : e.vf.x;
				xM = (yM == e.vi.y) ? e.vi.x : e.vf.x;
				et.mp[ym].intersections.push_back(Intersection(yM, xm, (double)(xm - xM) / (ym - yM)));
			}
		}

		for (auto& x : et.mp)
		{
			std::sort(x.second.intersections.begin(), x.second.intersections.end(),
			          [](const Intersection& i1, const Intersection& i2)
			          {
				          return i1.xMin < i2.xMin;
			          });
		}
	}

	void polyComputeSSM(Polygon& p, ET& et, ET& finalET)
	{
		Intersections activeSSM;
		int y, k;

		y = -1;
		for (int i = 0; i <= linesCount; ++i)
		{
			if (!et.mp.count(i))
			{
				y = i;
				break;
			}
		}
		if (y == -1)
		{
			return;
		}

		do
		{
			for (auto& intersection : et.mp[y].intersections)
			{
				activeSSM.intersections.push_back(intersection);
			}

			int index = 0;
			while (index < activeSSM.intersections.size())
			{
				if (activeSSM.intersections[index].yMax == y)
				{
					activeSSM.intersections.erase(activeSSM.intersections.begin() + index);
				}
				else
				{
					++index;
				}
			}

			k = activeSSM.intersections.size();
			sort(activeSSM.intersections.begin(), activeSSM.intersections.end(),
			     [](const Intersection& i1, const Intersection& i2)
			     {
				     return i1.xMin < i2.xMin;
			     });

			for (auto x : activeSSM.intersections)
			{
				finalET.mp[y].intersections.push_back(x);
			}
			++y;

			for (int i = 0; i < activeSSM.intersections.size(); ++i)
			{
				if (activeSSM.intersections[i].ratio != 0)
				{
					activeSSM.intersections[i].xMin += activeSSM.intersections[i].ratio;
				}
			}
		}
		while (activeSSM.intersections.size() || (y <= linesCount && et.mp[y].intersections.size()));
	}

	void polyColor(Polygon& p, ET& ssms)
	{
		std::set<std::pair<int, int>> allPoints = p.getPoints();
		std::set<std::pair<int, int>> displPoints = p.getToDisplayPoints();
		auto normalize = [](double value, int index) -> int
		{
			double vInt = int(value);
			if (abs(value - vInt) < 1e-8)
			{
				return int(value);
			}
			if (index % 2 == 1)
			{
				return int(floor(value));
			}
			return int(ceil(value));
		};

		for (auto x : ssms.mp)
		{
			int line = x.first;
			bool in = false;

			for (int i = 0; i < x.second.intersections.size() - 1; i += 2)
			{
				int from = normalize(x.second.intersections[i].xMin, i);
				int to = normalize(x.second.intersections[i + 1].xMin, i);
				if (from == to)
				{
					++to;
				}
				for (int j = from; j < to; ++j)
				{
					if (allPoints.count(std::make_pair(line, j)))
					{
						if (displPoints.count(std::make_pair(line, j)))
						{
							displayPixel(line, j);
						}
					}
					else
					{
						displayPixel(line, j);
					}
				}
			}
		}
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
		double radius = minSide / 4.0;

		glPolygonMode(GL_FRONT, GL_FILL);
		DisplayConvexPolygon(radius, x, y, 20, []()
		{
			glColor3f(0.05, 0.05, 0.05); // negru
		});
	}

	void displayPolygon(const std::string& fileName)
	{
		FILE* fileIn = fopen(fileName.c_str(), "r");
		if (!fileIn)
		{
			std::string message = "Invalid file name: " + fileName + "!";
			std::cout << message << '\n';
			throw std::invalid_argument(message);
		}
		Polygon p;
		polyRead(fileIn, p);
		polyDisplay(p);
		fclose(fileIn);

		ET et, ssms;
		polyInitET(p, et);
		polyComputeSSM(p, et, ssms);
		polyColor(p, ssms);
	}

	void displayCircleMarker(int originLine, int originColumn, int radius)
	{
		double minSide = getSide();
		double x, y;
		double size = radius * minSide;

		setCoordinateXY(originLine, originColumn, x, y, minSide);
		glPolygonMode(GL_FRONT, GL_LINE);
		glLineWidth(5);
		DisplayConvexPolygon(size, x, y, 360, []()
		{
			glColor3f(0.95, 0.05, 0.05); // negru
		});
		glLineWidth(1);
	}

	void displayEllipseMarker(int originLine, int originColumn, int a, int b)
	{
		double minSide = getSide();
		double x, y;
		double aTransposed = a * minSide;
		double bTransposed = b * minSide;
		setCoordinateXY(originLine, originColumn, x, y, minSide);
		glPolygonMode(GL_FRONT, GL_LINE);
		glLineWidth(5);
		DisplayEllipse(aTransposed, bTransposed, x, y, 360, []()
		{
			glColor3f(0.95, 0.05, 0.05); // negru
		});
		glLineWidth(1);
	}

	std::vector<Point> getCircleOutlinePoints(int radius, int size)
	{
		std::vector<Point> points;
		int x = radius, y = 0;
		int d = 1 - radius;
		int dN = 3, dNW = -2 * radius + 5;
		points.emplace_back(y, x);
		for (int i = 1; i <= size / 2; ++i)
		{
			points.emplace_back(y, x - i);
			points.emplace_back(y, x + i);
		}
		while (x > y)
		{
			if (d < 0)
			{
				d += dN;
				dN += 2;
				dNW += 2;
			}
			else
			{
				d += dNW;
				dN += 2;
				dNW += 4;
				x--;
			}
			y++;
			points.emplace_back(y, x);
			for (int i = 1; i <= size / 2; ++i)
			{
				points.emplace_back(y, x - i);
				points.emplace_back(y, x + i);
			}
		}
		return points;
	}

	void addEllipsePoints(int x, int y, std::vector<Point>& points, int originLine, int originColumn)
	{
		points.emplace_back(Point(y + originLine, x + originColumn));
		points.emplace_back(Point(-y + originLine, -x + originColumn));
		points.emplace_back(Point(-y + originLine, x + originColumn));
		points.emplace_back(Point(y + originLine, -x + originColumn));
	}

	std::vector<Point> getEllipsePoints(int a, int b, int originLine, int originColumn)
	{
		std::vector<Point> points;
		int x = 0, y = -b;
		double d1 = b * b + a * a * b + a * a / 4.0;
		double d2;

		addEllipsePoints(x, y, points, originLine, originColumn);
		while (a * a * (y - 0.5) < b * b * (x + 1))
		{
			if (d1 < 0)
			{
				d1 += b * b * (-2 * x - 3);
				x--;
				addEllipsePoints(x, y, points, originLine, originColumn);
			}
			else
			{
				d1 += b * b * (-2 * x + 3) + a * a * (2 * y + 2);
				x--;
				y++;
				for (int i = x; i <= 0; i++)
				{
					addEllipsePoints(i, y, points, originLine, originColumn);
				}
			}
		}
		
		d2 = b * b * (x - 0.5) * (x - 0.5) + a * a * (y + 1) * (y + 1) - a * a * b * b;
		while (y < 0)
		{
			if (d2 < 0)
			{
				d2 += b * b * (-2 * x + 2) + a * a * (2 * y + 3);
				x--;
				y++;
			}
			else
			{
				d2 += a * a * (2 * y + 3);
				y++;
			}
			for (int i = x; i <= 0; i++)
			{
				addEllipsePoints(i, y, points, originLine, originColumn);
			}
		}
		return points;
	}

	void displayCircleOutline(int originLine, int originColumn, int radius, int size)
	{
		displayCircleMarker(originLine, originColumn, radius);
		std::vector<Point> points = getCircleOutlinePoints(radius, size);
		for (auto p : points)
		{
			displayPixel(p.y, p.x, false);
		}
	}

	void displayFilledEllipse(int originLine, int originColumn, int a, int b)
	{
		displayEllipseMarker(originLine, originColumn, a, b);
		std::vector<Point> points = getEllipsePoints(a, b, originLine, originColumn);
		for (auto p : points)
		{
			displayPixel(p.y, p.x, false);
		}
	}
};

void Display0()
{
	CartesianGrid cg(15, 15);
	cg.displayGrid();
	cg.displayCircleOutline(0, 0, 13, 3);
}

void Display1()
{
	CartesianGrid cg(28, 26);
	cg.displayGrid();
	cg.displayFilledEllipse(7, 13, 13, 7);
}

void Display2()
{
	CartesianGrid cg(15, 15);
	cg.displayGrid();
	cg.displayPolygon("Poly1.txt");
}

void Display3()
{
	CartesianGrid cg(13, 13);
	cg.displayGrid();
	cg.displayPolygon("Poly2.txt");
}

void Display4()
{
	CartesianGrid cg(10, 10);
	cg.displayGrid();
	cg.displayPolygon("Poly3.txt");
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
	case '3':
		Display3();
		break;
	case '4':
		Display4();
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
