#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <variables.h>
#include <graphics.h>

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
		if (laneDestroyed[i])
			continue;
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
	glTranslatef(0, -1.02, 0);
	glScalef(gridRows, 1.0f, gridCols);
	glutSolidCube(2);
	glPopMatrix();
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0, 0, zoom);
	glRotatef(rotx, 1, 0, 0);
	glRotatef(roty, 0, 1, 0);
	glRotatef(rotz, 0, 0, 1);
	if (!paused) {
		detectBoltIntersections();
		detectMonstersIntersections();
	}
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
}

void Mouse(int b, int s, int x, int y)
{
	if (view || paused) {
		return;
	}
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
			currView = 0;
		}
		else {
			rotx = 30;
			roty = 90;
			zoom = -20.0f;
		}
		
	}
	if (view) {
		return;
	}
	if (key=='=')
	{
		// Zoom in
		if (zoom<-10)
			zoom += 0.3f;
		
	}
	if(key=='-')
	{
		// Zoom out
		if (zoom>-20.0f)
			zoom -= 0.3f;

	}
}

void initTiles() {
	float currX =-3, currZ;
	for (int i = 0; i < gridRows; i++) {
		currZ = 4;
		monsterFactories[i] = MonsterFactory(currX, 0, currZ+1);
		laneDestroyed[i] = false;
		for (int j = 0; j < gridCols; j++) {
			if(j%2==i%2)
				tiles[i][j] = Tile(currX, 0 , currZ, 0.01f, 1.0f, 0.01f);
			else
				tiles[i][j] = Tile(currX, 0, currZ, 0.1f, 0.1f, 0.7f);
			currZ -= 1;
		}
		tiles[i][7].addCharacter('d');
		currX += 1.2;	
	}
	
}

void addView(int index, float rX, float rY, float Z) {
	if (index > MAX_NUM_OF_VIEWS)
		return;
	views[0][index] = rX;
	views[1][index] = rY;
	views[2][index] = Z;
}

void initViews() {
	addView(0, 20, 0, -20);
	addView(1, 10, 90, -20);
	addView(2, 0, 180, -20);
}
int compare(float a, float b) {
	if (fabs(a - b) < 1.0)
		return 0;
	if (a > b)
		return 1;
	return -1;
}
void updateView() {
	bool done = true;
	if ((xDir>0&&rotx < views[0][currView])|| (xDir<0 && rotx > views[0][currView])) {
		if (xDir > 0)
			rotx += dx;
		else
			rotx -= dx;
		done = false;
	}
	if ((yDir>0&&roty < views[1][currView])|| (yDir<0 && roty > views[1][currView])) {
		if (yDir > 0)
			roty += dy;
		else
			roty -= dy;
		done = false;
	}
	if ((zoomDir>0&&zoom < views[2][currView])|| (zoomDir<0 && zoom > views[2][currView])) {
		if (zoomDir > 0)
			zoom += dzoom;
		else
			zoom -= dzoom;
		done = false;
	}
	if (done) {
		currView = (currView + 1) % MAX_NUM_OF_VIEWS;
		xDir = compare(views[0][currView] , rotx);
		yDir = compare(views[1][currView],  roty);
		zoomDir = compare(views[2][currView], zoom);
	}
}
void light() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}
void Anim() {
	glLoadIdentity();
	gluLookAt(7.0f, 7.0f, 0.0f, 0.0f, 0.0f, 0.01f, 0.0f, 1.0f, 0.0f);
	//glutPostRedisplay();
}
void timerFunc(int v)    
{

	if (view) {
		updateView();
	}
	glutPostRedisplay();
	glutTimerFunc(30, timerFunc, 0);
}
void main(int argc, char** argv) {
	
	initTiles();
	initViews();
	glutInit(&argc, argv);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(350, 100);

	glutCreateWindow("Plants vs Zombies");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutKeyboardFunc(key);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(0, timerFunc, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	

	glEnable(GL_DEPTH_TEST);
	light();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glClearColor(1.0f,1.0f,1.0f,0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 100.0f);

	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}
