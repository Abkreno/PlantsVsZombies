#include <glut.h>
#include <stdio.h>
#include <graphics.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

float zoom = 13.0f;
float rotx = 45;
float roty = 90.001f;
int lastx = 0;
int lasty = 0;
unsigned char Buttons[3] = { 0 };
const int gridRows = 5, gridCols = 9;

Tile tiles[gridRows][gridCols];


void drawGrid() {
	
	glPushMatrix();
	
	for (int i = 0; i < gridRows; i++) {
		for (int j = 0; j < gridCols; j++) {
			tiles[i][j].draw();
		}
	}
	
	glColor3f(0.2, 0.1, 0.2);
	glScalef(gridRows, 0.01f, gridCols);
	glutSolidCube(1);
	glPopMatrix();
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0, 0, -zoom);
	glRotatef(rotx, 1, 0, 0);
	glRotatef(roty, 0, 1, 0);

	drawGrid();
	glFlush();
}

void Motion(int x, int y)
{
	int diffx = x - lastx;
	int diffy = y - lasty;
	lastx = x;
	lasty = y;
	if (Buttons[0])
	{
		rotx += (float) 0.5f * diffy;
		roty += (float) 0.5f * diffx;
	}
	glutPostRedisplay();
}

void Mouse(int b, int s, int x, int y)
{
	lastx = x;
	lasty = y;
	switch (b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN == s) ? 1 : 0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN == s) ? 1 : 0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN == s) ? 1 : 0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Anim() {
	glLoadIdentity();
	gluLookAt(7.0f, 7.0f, 0.0f, 0.0f, 0.0f, 0.01f, 0.0f, 1.0f, 0.0f);
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y) {
}

void initTiles() {
	float currX =-2, currZ;
	for (int i = 0; i < gridRows; i++) {
		currZ = 4;
		for (int j = 0; j < gridCols; j++) {
			tiles[i][j] = Tile(currX, 0 , currZ, 0.1, 0.1, 0.1);
			currZ -= 1;
		}
		currX += 1;
	}
}

void main(int argc, char** argv) {
	
	initTiles();
	glutInit(&argc, argv);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(350, 100);

	glutCreateWindow("Plants vs Zombies");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutKeyboardFunc(key);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 600.0f);

	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}
