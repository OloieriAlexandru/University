#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <string>

#include "GL/freeglut.h"

// #include "glut.h" - Olo

// dimensiunea ferestrei in pixeli
#define dim 300

// numarul maxim de iteratii pentru testarea apartenentei la mult. Mandelbrot
#define NRITER_M 1000
// modulul maxim pentru testarea apartenentei la mult. Mandelbrot
#define MODMAX_M 2
// ratii ptr. Mandelbrot
#define RX_M 0.01
#define RY_M 0.01

unsigned char prevKey;
int nivel = 0;

class C2coord
{
public:
	C2coord()
	{
		m.x = m.y = 0;
	}

	C2coord(double x, double y)
	{
		m.x = x;
		m.y = y;
	}

	C2coord(const C2coord& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
	}

	C2coord& operator=(C2coord& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator==(C2coord& p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

protected:
	struct SDate
	{
		double x, y;
	} m;
};

class CPunct : public C2coord
{
public:
	CPunct() : C2coord(0.0, 0.0)
	{
	}

	CPunct(double x, double y) : C2coord(x, y)
	{
	}

	CPunct& operator=(const CPunct& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	void getxy(double& x, double& y)
	{
		x = m.x;
		y = m.y;
	}

	int operator==(CPunct& p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	void marcheaza()
	{
		glBegin(GL_POINTS);
		glVertex2d(m.x, m.y);
		glEnd();
	}

	void print(FILE* fis)
	{
		fprintf(fis, "(%+f,%+f)", m.x, m.y);
	}
};

class CVector : public C2coord
{
public:
	CVector() : C2coord(0.0, 0.0)
	{
		normalizare();
	}

	CVector(double x, double y) : C2coord(x, y)
	{
		normalizare();
	}

	CVector& operator=(CVector& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator==(CVector& p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	CPunct getDest(CPunct& orig, double lungime)
	{
		double x, y;
		orig.getxy(x, y);
		CPunct p(x + m.x * lungime, y + m.y * lungime);
		return p;
	}

	void rotatie(double grade)
	{
		double x = m.x;
		double y = m.y;
		double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
		m.x = x * cos(t) - y * sin(t);
		m.y = x * sin(t) + y * cos(t);
		normalizare();
	}

	void deseneaza(CPunct p, double lungime)
	{
		double x, y;
		p.getxy(x, y);
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x + m.x * lungime, y + m.y * lungime);
		glEnd();
	}

	void print(FILE* fis)
	{
		fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
	}

private:
	void normalizare()
	{
		double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
		if (d != 0.0)
		{
			C2coord::m.x = C2coord::m.x * 1.0 / d;
			C2coord::m.y = C2coord::m.y * 1.0 / d;
		}
	}
};

class CPatratTema
{
	void desenare_patrat(double lungime, CPunct& p, CVector& v)
	{
		for (int j = 0; j < 4; j++)
		{
			v.deseneaza(p, lungime / 3);
			p = v.getDest(p, lungime / 3);
			v.rotatie(-90);
		}
	}

public:
	void patratTema(double lungime,
		int nivel,
		CPunct p,
		CVector v)
	{
		assert(lungime / 3 != 0);
		CPunct p1, p2;
		if (nivel == 0)
		{
		}
		else
		{
			v.rotatie(135);
			CPunct p1 = v.getDest(p, sqrt(2 * (2 * lungime / 3) * (2 * lungime / 3)));
			v.rotatie(-135);
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					desenare_patrat(lungime, p1, v);
					patratTema(lungime / 3, nivel - 1, p1, v);
					v.rotatie(-i * 90.0);
					p1 = v.getDest(p1, lungime);
					v.rotatie(i * 90.0);
				}
			}
		}
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(1.0, 0.0);
		CPunct p(-1.0, 2.0);
		v.deseneaza(p, lungime);
		p = v.getDest(p, lungime);
		for (int i = 0; i < 3; i++)
		{
			v.rotatie(-90);
			v.deseneaza(p, lungime);
			p = v.getDest(p, lungime);
		}

		v.rotatie(-135);
		p = v.getDest(p, sqrt(2 * (lungime / 3) * (lungime / 3)));
		v.rotatie(45);
		v.deseneaza(p, lungime / 3);
		p = v.getDest(p, lungime / 3);

		for (int i = 0; i < 3; i++)
		{
			v.rotatie(-90);
			v.deseneaza(p, lungime / 3);
			p = v.getDest(p, lungime / 3);
		}
		v.rotatie(-90);
		patratTema(lungime / 3, nivel, p, v);
	}
};

class CArboreTema
{
public:
	void arboreTema(double lungime,
		int nivel,
		double factordiviziune,
		CPunct p,
		CVector v)
	{
		assert(factordiviziune != 0);
		CPunct p1, p2;
		if (nivel == 0)
		{
		}
		else
		{
			v.rotatie(-45);
			v.deseneaza(p, lungime);
			p1 = v.getDest(p, lungime);
			arboreTema(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			v.rotatie(90);
			v.deseneaza(p, lungime);
			p1 = v.getDest(p, lungime);
			p2 = p1;

			v.rotatie(10);
			v.deseneaza(p1, lungime);
			p1 = v.getDest(p1, lungime);
			arboreTema(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			p1 = p2;
			v.rotatie(-55);
			v.deseneaza(p1, lungime);
			p1 = v.getDest(p1, lungime);
			p2 = p1;

			v.rotatie(-90);
			v.deseneaza(p1, lungime / 2);
			p1 = v.getDest(p1, lungime / 2);
			arboreTema(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			p1 = p2;
			v.rotatie(120);
			v.deseneaza(p1, lungime / 2);
			p1 = v.getDest(p1, lungime / 2);
			arboreTema(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
		}
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(0.0, -1.0);
		CPunct p(-0.25, 2.5);

		v.deseneaza(p, 0.25);
		p = v.getDest(p, 0.25);
		arboreTema(lungime, nivel, 0.4, p, v);
	}
};

class CComplex {
public:
	CComplex() : re(0.0), im(0.0) {}
	CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
	CComplex(const CComplex& c) : re(c.re), im(c.im) {}
	~CComplex() {}

	CComplex& operator=(const CComplex& c)
	{
		re = c.re;
		im = c.im;
		return *this;
	}

	double getRe() { return re; }
	void setRe(double re1) { re = re1; }

	double getIm() { return im; }
	void setIm(double im1) { im = im1; }

	double getModul() { return sqrt(re * re + im * im); }

	int operator==(CComplex& c1)
	{
		return ((re == c1.re) && (im == c1.im));
	}

	CComplex pow2()
	{
		CComplex rez;
		rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
		rez.im = 2.0 * re * im;
		return rez;
	}

	friend CComplex operator+(const CComplex& c1, const CComplex& c2);
	friend CComplex operator*(CComplex& c1, CComplex& c2);

	void print(FILE* f)
	{
		fprintf(f, "%.20f%+.20f i", re, im);
	}

private:
	double re, im;
};

class CTriunghiTema
{
public:
	void triunghiTema(double lungime,
		int nivel,
		CPunct& p,
		CVector& v,
		int d)
	{
		if (nivel == 0)
		{
			v.deseneaza(p, lungime);
			p = v.getDest(p, lungime);
		}
		else
		{
			v.rotatie(d * 60.0);
			triunghiTema(lungime, nivel - 1, p, v, -d);

			v.rotatie(-d * 60.0);
			triunghiTema(lungime, nivel - 1, p, v, d);

			v.rotatie(-d * 60.0);
			triunghiTema(lungime, nivel - 1, p, v, -d);

			v.rotatie(d * 60.0);
		}
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(0.0, 1.0);
		v.rotatie(180.0);

		CPunct p(-0.6, 2.4);

		triunghiTema(lungime, nivel, p, v, 1);
	}
};

class CMandelbrot {
private:
	void display(double xMin, double yMin, double xMax, double yMax, double div,
		void(*isInCallback)(double, double, double, int), void(*isNotInCallback)(double, double, double, int))
	{
		glPushMatrix();
		glLoadIdentity();

		glBegin(GL_POINTS);
		for (double x = xMin; x <= xMax; x += RX_M)
		{
			for (double y = yMin; y <= yMax; y += RY_M)
			{
				int in = isIn(CComplex(x, y));
				if (!in)
				{
					if (isInCallback != nullptr)
					{
						isInCallback(x, y, div, in);
					}
				}
				else
				{
					if (isNotInCallback != nullptr)
					{
						isNotInCallback(x, y, div, in);
					}
				}
			}
		}
		glEnd();

		glPopMatrix();
	}
public:
	CMandelbrot()
	{
		m.nrIter = NRITER_M;
		m.mod = MODMAX_M;
	}

	CMandelbrot(int iter)
	{
		m.nrIter = iter;
		m.mod = MODMAX_M;
	}

	void setNrIter(int nrIter)
	{
		m.nrIter = nrIter;
	}

	int isIn(const CComplex& testedValue)
	{
		CComplex lastZ = CComplex();
		for (int i = 1; i <= m.nrIter; ++i) {
			if (lastZ.getModul() >= m.mod) {
				return i;
			}
			lastZ = lastZ.pow2() + testedValue;
		}
		return -0;
	}

	void displayPointsWhichBelong(double xMin, double yMin, double xMax, double yMax, double div = 1.0)
	{
		display(xMin, yMin, xMax, yMax, div, [](double x, double y, double div, int iterations) {
			glColor3f(1.0, 0.1, 0.1);
			glVertex3d(x / div, y / div, 0);
		}, nullptr);
	}

	void displayPointsWhichDoNotBelong(double xMin, double yMin, double xMax, double yMax, double div = 1.0)
	{
		display(xMin, yMin, xMax, yMax, div, nullptr, [](double x, double y, double div, int iterations) {
			glColor3f(1.0 / iterations, 0.1, 0.1);
			glVertex3d(x / div, y / div, 0);
		});
	}
private:
	struct SDate {
		int nrIter;
		double mod;
	} m;
};

CComplex operator+(const CComplex& c1, const CComplex& c2)
{
	CComplex rez(c1.re + c2.re, c1.im + c2.im);
	return rez;
}

CComplex operator*(CComplex& c1, CComplex& c2)
{
	CComplex rez(c1.re * c2.re - c1.im * c2.im,
		c1.re * c2.im + c1.im * c2.re);
	return rez;
}

std::string intToString(int value) {
	std::string res;
	if (value < 0) {
		res.push_back('-');
		value = -value;
	}
	while (value >= 10) {
		res.push_back((value % 10) + '0');
		value /= 10;
	}
	res.push_back((value % 10) + '0');
	std::reverse(res.begin(), res.end());
	return res;
}

void DisplayText(const std::string& imageName) {
	std::string levelStr = "Nivel=" + intToString(nivel);
	glRasterPos2d(-0.98, -0.98);
	for (int i = 0; i < levelStr.size(); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, levelStr[i]);
	}

	glRasterPos2d(-1.0, -0.9);
	for (int i = 0; i < imageName.size(); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, imageName[i]);
	}
}

void Display1()
{
	CPatratTema cpt;
	DisplayText("Patrat Tema");

	glPushMatrix();
	glLoadIdentity();
	glScaled(0.4, 0.4, 1);
	glTranslated(-0.5, -0.5, 0.0);
	cpt.afisare(3, nivel);
	glPopMatrix();
	nivel++;
}

void Display2()
{
	CArboreTema cat;
	DisplayText("Arbore Tema");

	glPushMatrix();
	glLoadIdentity();
	glScaled(0.4, 0.4, 1);
	glTranslated(-0.5, -0.5, 0.0);
	cat.afisare(1, nivel);
	glPopMatrix();
	nivel++;
}

void Display3()
{
	CTriunghiTema ctt;
	DisplayText("Triunghi Tema");

	glPushMatrix();
	glLoadIdentity();
	glScaled(0.4, 0.4, 1);
	glTranslated(-0.5, -0.5, 0.0);
	int mul = nivel + 2;
	ctt.afisare(1.8 / (mul * (mul + 1) / 2), nivel);

	glPopMatrix();
	nivel++;
}

void DisplayMandelbrot1()
{
	CMandelbrot cm(15);
	double div = 1.88;
	double sz = 1.0 * div;
	cm.displayPointsWhichBelong(-sz, -sz, sz, sz, div);
}

void DisplayMandelbrot2()
{
	CMandelbrot cm(15);
	double div = 1.88;
	double sz = 1.0* div;
	cm.displayPointsWhichDoNotBelong(-sz, -sz, sz, sz, div);
}

void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	glPointSize(3);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey)
	{
	case '0':
		nivel = 0;
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
		DisplayMandelbrot1();
		break;
	case '5':
		DisplayMandelbrot2();
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
