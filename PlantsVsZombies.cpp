#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <graphics.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600
float zoom = 20.0f;
float rotx = 30;
float roty = 90.001f;
float rotz = 0;
int lastx = 0;
int lasty = 0;
unsigned char Buttons[3] = { 0 };

bool view = false;

void drawHouse() {
	//Roof
	glPushMatrix();
	glColor3f(0.1f, 0.01f, 0.5f);
	glTranslatef(0.0f, 2.0f, -5.5f);
	glScalef(4.0f, 1.0f, 2.0f);
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	glutSolidOctahedron();
	glPopMatrix();

	//Base
	glPushMatrix();
	glColor3f(0.1f, 0.2f, 0.2f);
	glScalef(2.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 1.0f, -5.5f);
	glutSolidCube(2);
	glPopMatrix();
}
void drawGrid() {
	
	glPushMatrix();
	for (int i = 0; i < gridRows; i++) {
		
		for (int j = 0; j < gridCols; j++) {
			monsterFactories[i].drawMonsters();
			tiles[i][j].draw();
			
			if (monsterFactories[i].hasMonster) {
				tiles[i][j].defender.startAttack();
			}
			else {
				tiles[i][j].defender.stopAttack();
			}
		}
	}
	
	glColor3f(0.01, 0.5, 0.01);
	glTranslatef(0, -1.1, 0);
	glScalef(gridRows, 1.0f, gridCols);
	glutSolidCube(2);
	glPopMatrix();
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0, 0, -zoom);
	glRotatef(rotx, 1, 0, 0);
	glRotatef(roty, 0, 1, 0);
	glRotatef(rotz, 0, 0, 1);

	detectBoltIntersections();
	drawHouse();
	drawGrid();
	glFlush();
}

void Motion(int x, int y)
{
	if (view)
		return;
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
	if (view)
		return;
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
	if (key == 'f')monsterFactories[0].addMonster();
	if (key == 'p' || key == 'P') {
		paused = !paused;
	}
	if (paused) {
		return;
	}
	if (key == 'v') {
		view = !view;
		if (view) {
			rotx = 45;
			roty = 180;
		}
		else {
			rotx = 30;
			roty = 90;
		}
		
	}
	if (view) {
		return;
	}
	if (key=='=')
	{
		// Zoom in
		if(zoom>1)
			zoom -= 0.3f;

	}
	if(key=='-')
	{
		// Zoom out
		if(zoom<22)
			zoom += 0.3f;
	}
}

void initTiles() {
	float currX =-2, currZ;
	for (int i = 0; i < gridRows; i++) {
		currZ = 4;
		monsterFactories[i] = MonsterFactory(currX, 0, currZ+1);
		for (int j = 0; j < gridCols; j++) {
			if(j%2==i%2)
				tiles[i][j] = Tile(currX, 0 , currZ, 0.01, 1, 0.01);
			else
				tiles[i][j] = Tile(currX, 0, currZ, 0.1f, 0.1f, 0.3f);
			currZ -= 1;
		}
		tiles[i][7].addCharacter('d');
		currX += 1;	
	}
}
void light() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	
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
	light();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 100.0f);

	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}
