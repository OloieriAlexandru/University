#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "GL/freeglut.h"

// #include "glut.h" - Olo

// dimensiunea ferestrei in pixeli
#define dim 300
#define PI 3.14159265

unsigned char prevKey;


enum EObiect { cubw, cubs, sferaw, sferas } ob = cubw;

class Point3D
{
public:
	float x;
	float y;
	float z;

	Point3D(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

	void display()
	{
		glVertex3f(this->x, this->y, this->z);
	}
};

class Triangle
{
public:
	Point3D p1, p2, p3;

	Triangle(Point3D p1, Point3D p2, Point3D p3) : p1(p1), p2(p2), p3(p3)
	{
	}

	void display()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLES);
		p1.display();
		p2.display();
		p3.display();
		glEnd();
	}
};

Triangle t(Point3D(2, 2, 1.0), Point3D(7, 3, 4.5), Point3D(7, 8, 5.0));

void DisplayTriangle()
{
	glColor3f(1.0f, 0.00, 0.00);
	t.display();
}

void DisplayTriangle1()
{
	glColor3f(1.0f, 0.00, 0.00);
	glTranslatef(-t.p1.x, -t.p1.y, -t.p1.z);
	t.display();
}

void DisplayTriangle2()
{
	glColor3f(1.0f, 0.00, 0.00);

	float tan = (t.p2.z - t.p1.z) / (t.p2.x - t.p1.x);
	float theta = atanf(tan) * 180 / PI;

	int xSign = t.p2.x - t.p1.x < 0 ? -1 : 1;
	int zSign = t.p2.z - t.p1.z < 0 ? -1 : 1;
	theta = -xSign * (90.0 - zSign * fabs(theta));
	glRotatef(theta, 0.0, 1.0, 0.0);
	glTranslatef(-t.p1.x, -t.p1.y, -t.p1.z);

	t.display();
}


void DisplayTriangle3()
{
	glColor3f(1.0f, 0.00, 0.00);

	float D1 = sqrtf((t.p2.x - t.p1.x) * (t.p2.x - t.p1.x) + (t.p2.z - t.p1.z) * (t.p2.z - t.p1.z));
	float D2 = sqrtf((t.p2.x - t.p1.x) * (t.p2.x - t.p1.x) +
		(t.p2.z - t.p1.z) * (t.p2.z - t.p1.z) +
		(t.p2.y - t.p1.y) * (t.p2.y - t.p1.y));
	float tan_theta = (t.p2.z - t.p1.z) / (t.p2.x - t.p1.x);
	float theta = atanf(tan_theta) * 180 / PI;
	float xSign = t.p2.x - t.p1.x < 0.0 ? -1.0 : 1.0;
	float zSign = t.p2.z - t.p1.z < 0.0 ? -1.0 : 1.0;
	printf("%f\n", theta);
	theta = -xSign * (90.0 - zSign * fabs(theta));
	printf("%f\n", theta);
	float sin = (t.p2.y - t.p1.y) / D2;
	float cos = D1 / D2;

	float tan_phi = sin / cos;
	float phi = atanf(tan_phi) * 180 / PI;

	glRotatef(phi, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	glTranslatef(-t.p1.x, -t.p1.y, -t.p1.z);

	t.display();
}

void DisplayTriangle4()
{
	glColor3f(1.0f, 0.00, 0.00);

	float D1 = sqrtf((t.p2.x - t.p1.x) * (t.p2.x - t.p1.x) + (t.p2.z - t.p1.z) * (t.p2.z - t.p1.z));
	float D2 = sqrtf((t.p2.x - t.p1.x) * (t.p2.x - t.p1.x) +
		(t.p2.z - t.p1.z) * (t.p2.z - t.p1.z) +
		(t.p2.y - t.p1.y) * (t.p2.y - t.p1.y));

	float x3 = ((t.p2.z - t.p1.z) * (t.p3.x - t.p1.x) - (t.p2.x - t.p1.x) * (t.p3.z - t.p1.z)) / D1;
	float y3 = (D1 * D1 * (t.p3.y - t.p1.y) - (t.p2.y - t.p1.y) * ((t.p2.x - t.p1.x) * (t.p3.x - t.p1.x) + (t.p2.z - t.
		p1.z) * (t.p3.z - t.p1.z))) / (D1 * D2);


	float D3 = sqrtf(x3 * x3 + y3 * y3);


	float tan_theta = (t.p2.z - t.p1.z) / (t.p2.x - t.p1.x);
	float theta = atanf(tan_theta) * 180 / PI;
	int xSign = t.p2.x - t.p1.x < 0 ? -1 : 1;
	int zSign = t.p2.z - t.p1.z < 0 ? -1 : 1;
	theta = -xSign * (90.0 - zSign * fabs(theta));

	float sin = (t.p2.y - t.p1.y) / D2;
	float cos = D1 / D2;
	float tan_phi = sin / cos;
	float phi = atanf(tan_phi) * 180 / PI;

	sin = x3 / D3;
	cos = x3 / D3;
	float alpha;

	if (cos == 0)
	{
		alpha = 0;
	}
	else
	{
		float tan_alpha = sin / cos;
		alpha = atanf(tan_alpha) * 180 / PI;
	}

	glRotatef(alpha, 0.0, 0.0, 1.0);
	glRotatef(phi, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	glTranslatef(-t.p1.x, -t.p1.y, -t.p1.z);

	t.display();
}

void getPhiTheta(Point3D& p1, Point3D& p2, float& phi, float& theta) {
	float D1 = sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.z - p1.z) * (p2.z - p1.z));
	float D2 = sqrtf((p2.x - p1.x) * (p2.x - p1.x) +
		(p2.z - p1.z) * (p2.z - p1.z) +
		(p2.y - p1.y) * (p2.y - p1.y));
	float tan_theta = (p2.z - p1.z) / (p2.x - p1.x);
	theta = atanf(tan_theta) * 180 / PI;
	float xSign = p2.x - p1.x < 0.0 ? -1.0 : 1.0;
	float zSign = p2.z - p1.z < 0.0 ? -1.0 : 1.0;

	// printf("%f\n", theta);
	
	theta = -xSign * (90.0 - zSign * fabs(theta));
	
	// printf("%f\n", theta);
	
	float sin = (p2.y - p1.y) / D2;
	float cos = D1 / D2;

	float tan_phi = sin / cos;
	phi = atanf(tan_phi) * 180 / PI;
}

float SZ = 8.0;

bool recenter = true;
int cubeType = 2;
bool addLine = true;

int angleGL = 0;
int angleO = 0;
int ANG = 30;

void manualCube(float SZ) {
	std::vector<std::vector<Point3D>> faces = {
		{
			Point3D(0, 0, 0),
			Point3D(SZ, 0, 0),
			Point3D(SZ, SZ, 0),
			Point3D(0, SZ, 0),
		},
		{
			Point3D(0, 0, 0),
			Point3D(0, 0, SZ),
			Point3D(0, SZ, SZ),
			Point3D(0, SZ, 0),
		},
		{
			Point3D(0, 0, 0),
			Point3D(SZ, 0, 0),
			Point3D(SZ, 0, SZ),
			Point3D(0, 0, SZ),
		},
		{
			Point3D(SZ, SZ, SZ),
			Point3D(0, SZ, SZ),
			Point3D(0, SZ, 0),
			Point3D(SZ, SZ, 0),
		},
		{	
			Point3D(SZ, SZ, SZ),
			Point3D(0, SZ, SZ),
			Point3D(0, 0, SZ),
			Point3D(SZ, 0, SZ),
		},
		{
			Point3D(SZ, SZ, SZ),
			Point3D(SZ, SZ, 0),
			Point3D(SZ, 0, 0),
			Point3D(SZ, 0, SZ),
		},
	};
	std::vector<Point3D> colors = {
		Point3D(1, 0, 0),
		Point3D(0, 1, 0),
		Point3D(0, 0, 1),
		Point3D(1, 1, 0),
		Point3D(0, 1, 1),
		Point3D(1, 0, 1)
	};

	for (int i = 0; i < 6; ++i) {
		glColor3f(colors[i].x, colors[i].y, colors[i].z);
		glBegin(GL_QUADS);
		for (auto x : faces[i]) {
			glVertex3f(x.x, x.y, x.z);
		}
		glEnd();
	}
}

void DisplayCube()
{
	glLineWidth(4);
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(SZ, SZ, SZ);
	glEnd();

	glLineWidth(1);
	glColor3f(0.4, 0.8, 0.2);
	if (cubeType == 0) {
		glTranslated(SZ / 2, SZ / 2, SZ / 2);
		glutWireCube(SZ);
	}
	else if (cubeType == 1) {
		glTranslated(SZ / 2, SZ / 2, SZ / 2);
		glutSolidCube(SZ);
	}
	else if (cubeType == 2) {
		manualCube(SZ);
		if (addLine) {
			glLineWidth(4);
			glColor3f(0.1, 0.1, 0.1);
			glBegin(GL_LINES);
			glVertex3f(0, 0, 0);
			glVertex3f(SZ, SZ, SZ);
			glEnd();
		}
	}
}

void DisplayCube1()
{
	DisplayCube();
}

void DisplayCube2()
{
	glRotated(angleGL++, 1, 1, 1);
	DisplayCube();
}

void DisplayCube3()
{
	Point3D p1(0.0, 0.0, 0.0);
	Point3D p2(SZ, SZ, SZ);

	float phi, theta;
	getPhiTheta(p1, p2, phi, theta);

	glRotatef(theta, 0.0, 1.0, 0.0);
	DisplayCube();
}

void DisplayCube4()
{
	Point3D p1(0.0, 0.0, 0.0);
	Point3D p2(SZ, SZ, SZ);

	float phi, theta;
	getPhiTheta(p1, p2, phi, theta);

	glRotatef(phi, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	DisplayCube();
}

void DisplayCube5()
{
	Point3D p1(0.0, 0.0, 0.0);
	Point3D p2(SZ, SZ, SZ);

	float phi, theta;
	getPhiTheta(p1, p2, phi, theta);

	glRotatef(ANG, 0.0, 0.0, 1.0);
	glRotatef(phi, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	DisplayCube();
}

void DisplayCube6()
{
	Point3D p1(0.0, 0.0, 0.0);
	Point3D p2(SZ, SZ, SZ);

	float phi, theta;
	getPhiTheta(p1, p2, phi, theta);

	glRotatef(-phi, 1.0, 0.0, 0.0);
	glRotatef(ANG, 0.0, 0.0, 1.0);
	glRotatef(phi, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	DisplayCube();
}

void DisplayCube7()
{
	Point3D p1(0.0, 0.0, 0.0);
	Point3D p2(SZ, SZ, SZ);

	float phi, theta;
	getPhiTheta(p1, p2, phi, theta);

	glRotatef(-theta, 0.0, 1.0, 0.0);
	glRotatef(-phi, 1.0, 0.0, 0.0);
	glRotatef(ANG, 0.0, 0.0, 1.0);
	glRotatef(phi, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	DisplayCube();
}

void DisplayCube8()
{
	Point3D p1(0.0, 0.0, 0.0);
	Point3D p2(SZ, SZ, SZ);

	float phi, theta;
	getPhiTheta(p1, p2, phi, theta);

	glRotatef(-theta, 0.0, 1.0, 0.0);
	glRotatef(-phi, 1.0, 0.0, 0.0);
	glRotatef(angleO++, 0.0, 0.0, 1.0);
	glRotatef(phi, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	DisplayCube();
}

void DisplayAxe()
{
	int X, Y, Z;
	X = Y = 200;
	Z = 200;

	glLineWidth(2);

	// axa Ox - verde
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(X, 0, 0);
	glEnd();

	// axa Oy - albastru
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, Y, 0);
	glEnd();

	// axa Oz - rosu
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, Z);
	glEnd();

	glLineWidth(1);
}

void Init(void)
{
	glClearColor(1, 1, 1, 1);
	glLineWidth(2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, 30, -30);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(20, 1, 0, 0);
	glRotated(-20, 0, 1, 0);
}

void doRotate() {
	if (recenter) {
		glPushMatrix();
		glRotated(-25, 0, 1, 0);
		glRotated(10, 1, 0, 0);
		glRotated(-10, 0, 0, 1);
	}
}

void doRerotate() {
	if (recenter) {
		glPopMatrix();
	}
}

void Display(void)
{
	switch (prevKey)
	{
	case 'a':
		DisplayAxe();
		break;
	case '0':
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(20, 1, 0, 0);
		glRotated(-20, 0, 1, 0);
		break;
	case 'q':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayAxe();
		DisplayTriangle();
		break;
	case 'w':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayAxe();
		glPushMatrix();
			DisplayTriangle1();
		glPopMatrix();
		break;
	case 'e':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayAxe();
		glPushMatrix();
			DisplayTriangle2();
		glPopMatrix();
		break;
	case 'r':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayAxe();
		glPushMatrix();
			DisplayTriangle3();
		glPopMatrix();
		break;
	case 't':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayAxe();
		glPushMatrix();
			DisplayTriangle4();
		glPopMatrix();
		break;
	case 'z':
		printf("z\n");
		glClear(GL_COLOR_BUFFER_BIT);
		doRotate();
		DisplayAxe();
		glPushMatrix();
			DisplayCube1();
		glPopMatrix();
		doRerotate();
		break;
	case 'x':
		printf("x\n");
		glClear(GL_COLOR_BUFFER_BIT);
		doRotate();
		DisplayAxe();
		glPushMatrix();
			DisplayCube2();
		glPopMatrix();
		doRerotate();
		break;
	case 'c':
		printf("c\n");
		glClear(GL_COLOR_BUFFER_BIT);
		doRotate();
		DisplayAxe();
		glPushMatrix();
			DisplayCube3();
		glPopMatrix();
		doRerotate();
		break;
	case 'v':
		printf("v\n");
		glClear(GL_COLOR_BUFFER_BIT);
		doRotate();
		DisplayAxe();
		glPushMatrix();
			DisplayCube4();
		glPopMatrix();
		doRerotate();
		break;
	case 'b':
		printf("b\n");
		glClear(GL_COLOR_BUFFER_BIT);
		doRotate();
		DisplayAxe();
		glPushMatrix();
			DisplayCube5();
		glPopMatrix();
		doRerotate();
		break;
	case 'n':
		printf("n\n");
		glClear(GL_COLOR_BUFFER_BIT);
		doRotate();
		DisplayAxe();
		glPushMatrix();
			DisplayCube6();
		glPopMatrix();
		doRerotate();
		break;
	case 'm':
		printf("m\n");
		glClear(GL_COLOR_BUFFER_BIT);
		doRotate();
		DisplayAxe();
		glPushMatrix();
			DisplayCube7();
		glPopMatrix();
		doRerotate();
		break;
	case ',':
		printf(",\n");
		glClear(GL_COLOR_BUFFER_BIT);
		doRotate();
		DisplayAxe();
		glPushMatrix();
			DisplayCube8();
		glPopMatrix();
		doRerotate();
		break;
	default:
		break;
	}
	glutSwapBuffers();
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
	/* initialize random seed: */
	srand(time(NULL));

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}
