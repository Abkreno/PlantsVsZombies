#include <glut.h>
#include <stdio.h>
#include <tile.h>


#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

const int gridRows = 5, gridCols = 7;

Tile tiles[gridRows][gridCols];


void drawGrid() {
	
	
	
	for (int i = 0; i < gridRows; i++) {
		for (int j = 0; j < gridCols; j++) {
			tiles[i][j].draw();
		}
	}
	glPushMatrix();
	glColor3f(0.2, 0.1, 0.2);
	glScalef(gridRows, 0.01f, gridCols);
	glutSolidCube(1);
	glPopMatrix();
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawGrid();
	glFlush();
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
		currZ = 3;
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, 600.0f);

	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}
