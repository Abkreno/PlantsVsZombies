#include <glut.h>
#include <stdio.h>
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

const int gridRows = 5, gridCols = 7;

struct Tile {
	float x, y, z, r, b, g;
	bool highlighted, occupied;
	Tile(float x, float y, float z, float r, float g, float b) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
		this->g = g;
		this->b = b;
		highlighted = false;
		occupied = false;
	}

	void draw() {
		glPushMatrix();
		glTranslatef(x, y, z);
		glColor3f(r, g, b);
		glScalef(1.0f, 0.05f, 1.0f);
		glutSolidCube(1);
		glPopMatrix();
	}
};
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glFlush();
}

void Anim() {
	glLoadIdentity();
	gluLookAt(7.0f, 7.0f, 0.0f, 0.0f, 0.0f, 0.01f, 0.0f, 1.0f, 0.0f);
	glutPostRedisplay();
}
void key(unsigned char key, int x, int y) {
	
}
void main(int argc, char** argv) {
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
