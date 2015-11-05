#include <glut.h>
#include <stdio.h>
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

struct Tile {
	float x, y, z, r, b, g;
	bool highlighted, occupied;
	Tile(float x, float y, float z, float r, float b, float g) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
		this->b = b;
		this->g = g;
		highlighted = false;
		occupied = false;
	}
};
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCube(1);
	glPopMatrix();
	glFlush();
}

void Anim() {
	glLoadIdentity();
	
	glutPostRedisplay();
}
void key(unsigned char key, int x, int y) {
	
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(350, 350);

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
