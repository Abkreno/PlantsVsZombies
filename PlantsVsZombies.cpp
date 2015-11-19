#include <string>
#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <variables.h>
#include <graphics.h>

using namespace std;

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
	glTranslatef(0.6, 0, 0);
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
	glPopMatrix();
}

void drawText(float x,float y,void* font ) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRasterPos2f(0, 0); // center of screen. (-1,0) is center left.
	const char * p = temp_buffer;
	do glutBitmapCharacter(font	, *p); while (*(++p));
	glPopMatrix();
}

void drawTexture() {
	glColor3f(0, 0, 0);
	//TEXT
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // save
	glLoadIdentity();// and clear
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST); // also disable the depth test so renders on top
	
	float y = 0.95;
	sprintf(temp_buffer, " Character      key        cost");
	drawText(-0.87, y, GLUT_BITMAP_HELVETICA_18);
	sprintf(temp_buffer, " Defender                  d              %d$", defender_cost);
	y -= 0.08;
	drawText(-0.87, y,GLUT_BITMAP_HELVETICA_12);
	sprintf(temp_buffer, " Resource Gatherer     r              0%d$", resource_gatherer_cost);
	y -= 0.08;
	drawText(-0.87, y, GLUT_BITMAP_HELVETICA_12);
	sprintf(temp_buffer, " Warrior                     w             %d$", warrior_cost);
	y -= 0.08;
	drawText(-0.87, y, GLUT_BITMAP_HELVETICA_12);
	sprintf(temp_buffer, " Shield                       s             %d$", sheild_cost);
	y -= 0.08;
	drawText(-0.87, y, GLUT_BITMAP_HELVETICA_12);
	
	sprintf(temp_buffer, "Money = %d", money);
	drawText(0.6, 0.9, GLUT_BITMAP_8_BY_13);

	glEnable(GL_DEPTH_TEST); // Turn depth testing back on
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // revert back to the matrix I had before.
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	

	glPushMatrix();
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
	glColor3f(0.01, 0.5, 0.01);
	glTranslatef(0, -1.02, 0);
	glScalef(gridRows, 1.0f, gridCols);
	glutSolidCube(2);
	glPopMatrix();

	drawTexture();

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

void switchView() {
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
void zoomIn() {
	// Zoom in
	if (zoom<0)
		zoom += 0.3f;
}
void zoomOut() {
	// Zoom out
	if (zoom>-20.0f)
		zoom -= 0.3f;
}
void handleCharacterInsertion(unsigned char key) {
	if (key == ESC_KEY){
		currState = SELECT_ROW;
		if (selected_row < 5 && selected_row>=0
			&& selected_col < 9 && selected_col>=0)
			tiles[selected_row][selected_col].highlighted = false;
		return;
	}
	if (currState == SELECT_ROW) {
		if (key > '0' && key <= '5') {
			selected_row = (key - '0') - 1;
			selected_col = 8;
			tiles[selected_row][selected_col].highlighted = true;
			currState = SELECT_COL;
		}
	}
	else if (key >= '0'&&key <= '9') {
		if (key > '0' && key <= '9') {
			tiles[selected_row][selected_col].highlighted = false;
			selected_col = (key - '0') - 1;
			selected_col = 8 - selected_col;
			tiles[selected_row][selected_col].highlighted = true;
		}
	}
	else {
		if (key == 'c') {
			tiles[selected_row][selected_col].destroyCharacter();
		}
		else if (key == 'd'||key=='r'||key=='w') {
			tiles[selected_row][selected_col].addCharacter(key);
		}
		tiles[selected_row][selected_col].highlighted = false;
		currState = SELECT_ROW;
	}
}
int counterrr = 0;
void key(unsigned char key, int x, int y) {
	if (key == 'k') {
		printf("addView(%d, %0.3f, %0.3f, %0.3f);\n", counterrr++, rotx, roty, zoom);
		
	}
	if (key == 'f')
		monsterFactories[0].addMonster();
	if (key == 'p' || key == 'P')
		paused = !paused;
	if (paused) 
		return;
	if (key >= '0'&&key <= '9' || key == 'r' || key == 'd' 
		|| key == 'w' || key == 'c' || key == ESC_KEY)
		handleCharacterInsertion(key);
	if (key == 'v')
		switchView();
	if (view) 
		return;
	if (key=='=')
		zoomIn();
	if(key=='-')
		zoomOut();
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
	addView(0, 36.000, 179.501, -20.000);
	addView(1, 32.000, 132.001, -20.000);
	addView(2, 5.500, 153.001, -11.000);
	addView(3, 9.000, 154.001, -5.600);
	addView(4, 35.500, 181.501, -5.600);
	addView(5, 6.000, 363.001, -5.600);
	addView(6, 6.000, 363.001, -12.500);
	addView(7, 7.000, 417.501, -12.500);
	addView(8, 6.500, 507.001, -12.500);
	addView(9, 87.500, 490.001, -12.500);
	addView(10, 21.500, 641.501, -12.500);
	addView(11, 2.000, 688.001, -12.500);
	addView(12, 1.500, 815.001, -20.000);
	addView(13, 2.000, 903.001, -20.000);
	addView(14, 1.500, 997.001, -20.000);
	addView(15, -1.500, 1081.001, -20.000);
	addView(16, 86.500, 1080.001, -20.000);
	addView(17, 76.500, 900.001, -20.000);

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
	if ((xDir>0&&rotx < views[0][currView])|| 
		(xDir<0 && rotx > views[0][currView])) {
		if (xDir > 0)
			rotx += dx;
		else
			rotx -= dx;
		done = false;
	}
	if ((yDir>0&&roty < views[1][currView])
		|| (yDir<0 && roty > views[1][currView])) {
		if (yDir > 0)
			roty += dy;
		else
			roty -= dy;
		done = false;
	}
	if ((zoomDir>0&&zoom < views[2][currView])
		|| (zoomDir<0 && zoom > views[2][currView])) {
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
	gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.01f, 0.0f, 1.0f, 0.0f);
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
