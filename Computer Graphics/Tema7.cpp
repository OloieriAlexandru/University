#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "GL/freeglut.h"

// #include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300
#define PI 3.14159265359
unsigned char prevKey;
GLint k;
// latura cubului
GLdouble lat = 5;

void proiectieParalela(unsigned char);
void DisplayAxe();
void InitObiect();
void DisplayObiect();

GLfloat cabinet[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			cos(PI / 8), sin(PI / 2) / 2, 1, 0,
			0, 0, 0, 1
};

void Init(void)
{
	glClearColor(1, 1, 1, 1);

	// validare test de adancime
	glEnable(GL_DEPTH_TEST);

	// se aloca 1 lista de display numerotata k
	k = glGenLists(1);
	InitObiect();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void Display()
{
	switch (prevKey)
	{
	case '0':
		// resetarea stivei matricilor de modelare si a celei de proiectie
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		DisplayAxe();
		break;
	case 'A':
		DisplayAxe();
		break;
	case 'C':
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
	case 'X':
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		proiectieParalela('X');
		glMatrixMode(GL_MODELVIEW);
		glRotatef(10, 1, 0, 0);
		DisplayAxe();
		DisplayObiect();
		break;
	case 'Y':
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		proiectieParalela('Y');
		glMatrixMode(GL_MODELVIEW);
		glRotatef(10, 0, 1, 0);
		DisplayAxe();
		DisplayObiect();
		break;
	case 'Z':
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		proiectieParalela('Z');
		glMatrixMode(GL_MODELVIEW);
		glRotatef(10, 0, 0, 1);
		DisplayAxe();
		DisplayObiect();
		break;
	case 'q':
		// proiectie paralela ortografica frontala (fata)
		proiectieParalela('q');
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		DisplayAxe();
		glTranslated(0, 0, -lat);
		DisplayObiect();
		break;
	case 'w':
		// proiectie paralela ortografica frontala (spate)
		proiectieParalela('w');
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslated(0, 0, -lat);
		glTranslated(lat / 2.0, lat / 2.0, lat / 2.0);
		glRotated(180, 0, 1, 0);
		glTranslated(-lat / 2.0, -lat / 2.0, -lat / 2.0);
		DisplayAxe();
		DisplayObiect();
		glPopMatrix();
		break;
	case 'e':
		proiectieParalela('e');
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslated(0, 0, -lat);
		glTranslated(lat / 2.0, lat / 2.0, lat / 2.0);
		glRotated(90, 1, 0, 0);
		glTranslated(-lat / 2.0, -lat / 2.0, -lat / 2.0);
		DisplayAxe();
		DisplayObiect();
		glPopMatrix();
		break;
	case 'r':

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glFrustum(-5, 5, -5, 5, 5, 20.0);
		glTranslated(-5, -5, -10);
		// glRotated(180, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		DisplayAxe();

		glPushMatrix();
		glTranslated(-5, -5, -10);
		glClear(GL_DEPTH_BUFFER_BIT);
		DisplayObiect();
		glPopMatrix();


		// glFrustum(1, 3, -1, 1, 5, 20.0);
		// glTranslated(2, 0, -10);
		// // glRotated(180, 0, 0, 0);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glMatrixMode(GL_MODELVIEW);
		//
		// glLoadIdentity();
		// DisplayAxe();
		// //glTranslated(-5, -5, -10);
		// //glClear(GL_DEPTH_BUFFER_BIT);
		// //DisplayObiect();
		break;
	case 't':
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(cabinet);
		glOrtho(-5, 5, -5, 5, -8, 15);
		glTranslatef(-3.5, -3.5, -7.5);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		DisplayAxe();
		DisplayObiect();
		break;
	default:
		break;
	}
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	h = (h == 0) ? 1 : h;
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

	// GLUT_DEPTH - eliminarea suprafetelor ascunse cu alg. Z-buffer
	glutInitDisplayMode(GL_COLOR_BUFFER_BIT | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);
	//   glutDisplayFunc(DisplayAll);

	glutMainLoop();

	return 0;
}

void proiectieParalela(unsigned char c)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (c)
	{
	case 'X':
	case 'Y':
	case 'Z':
		glOrtho(-10, 10, -10, 10, -20, 20);
		break;
	case 'q':
	case 'w':
	case 'e':
	case 't':
		glOrtho(-1, 6, -1, 6, -1, 20);
		break;
	default:
		break;
	}
}

void DisplayAxe()
{
	int X, Y, Z;
	X = Y = 200;
	Z = 200;
	glLineWidth(2);

	// axa Ox - verde
	glColor3f(0.1, 1, 0.1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(X, 0, 0);
	glEnd();

	// axa Oy - albastru
	glColor3f(0.1, 0.1, 1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, Y, 0);
	glEnd();

	// axa Oz - rosu
	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, Z);
	glEnd();

	glLineWidth(1);
}

void InitObiect()
{
	glNewList(k, GL_COMPILE);
	// fata 1
	glColor3f(1, 0, 0); // rosu
	glBegin(GL_QUADS);
	glVertex3d(0, lat, lat);
	glVertex3d(lat, lat, lat);
	glVertex3d(lat, 0, lat);
	glVertex3d(0, 0, lat);
	glEnd();
	// fata 2
	glColor3f(1, 1, 0); // galben
	glBegin(GL_QUADS);
	glVertex3d(lat, 0, 0);
	glVertex3d(lat, 0, lat);
	glVertex3d(lat, lat, lat);
	glVertex3d(lat, lat, 0);
	glEnd();
	// fata 3
	glColor3f(0, 1, 0); // verde
	glBegin(GL_QUADS);
	glVertex3d(0, lat, lat);
	glVertex3d(lat, lat, lat);
	glVertex3d(lat, lat, 0);
	glVertex3d(0, lat, 0);
	glEnd();
	// fata 4
	glColor3f(0, 0, 1); // albastru
	glBegin(GL_QUADS);
	glVertex3d(0, 0, 0);
	glVertex3d(lat, 0, 0);
	glVertex3d(lat, 0, lat);
	glVertex3d(0, 0, lat);
	glEnd();
	// fata 5
	glColor3f(1, 0, 1); // magenta
	glBegin(GL_QUADS);
	glVertex3d(0, 0, lat);
	glVertex3d(0, 0, 0);
	glVertex3d(0, lat, 0);
	glVertex3d(0, lat, lat);
	glEnd();
	// fata 6
	glColor3f(0, 1, 1); // cyan
	glBegin(GL_QUADS);
	glVertex3d(0, lat, 0);
	glVertex3d(lat, lat, 0);
	glVertex3d(lat, 0, 0);
	glVertex3d(0, 0, 0);
	glEnd();
	glEndList();
}

void DisplayObiect()
{
	glCallList(k);
}
