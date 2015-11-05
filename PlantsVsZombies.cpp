#include <glut.h>
#include <stdio.h>
float rotAng,angX,angY,angZ,angX2,angY2,angZ2;
void drawGrid() {

}
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//drawGrid();

	glPushMatrix();

	glColor3f(0.2, 0.1, 0.1);
	glPushMatrix();
	glScalef(1, 0.05, 1);
	glTranslatef(-2.0, 0, 2.0);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, 0.05, 1);
	glTranslatef(0.0, 0, 2.0);
	glutSolidCube(2);
	glPopMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	glScalef(1, 0.01, 1);
	glTranslatef(0, 0, 0);
	glutSolidCube(6);

	glPopMatrix();

	glPushMatrix();
	glRotatef(-rotAng, 0, 1, 0);
	glTranslatef(2, 0, 0);
	glRotatef(rotAng, 1, 0, 0);
	glColor3f(0.5f, 0.5f, 0.5f);
	//glutSolidSphere(0.5, 25, 25);
	glPopMatrix();

	glFlush();
}

void Anim() {
	rotAng += 0.01;
	glLoadIdentity();
	gluLookAt(angX, angY, angZ, angX2, angY2, angZ2, 0.0f, 1.0f, 0.0f);
	glutPostRedisplay();
}
void key(unsigned char key, int x, int y) {
	
	if (key == 49) {
		angX += 0.1;
	}
	if (key == 50) {
		angY += 0.1;
	}
	if (key == 51) {
		angZ += 0.1;
	}
	if (key == 52) {
		angX2 += 0.1;
	}
	if (key == 53) {
		angY2 += 0.1;
	}
	if (key == 54) {
		angZ2 += 0.1;
	}
	
	if (key == 113) {
		angX -= 0.1;
	}
	if (key == 119) {
		angY -= 0.1;
	}
	if (key == 101) {
		angZ -= 0.1;
	}
	if (key == 114) {
		angX2 -= 0.1;
	}
	if (key == 116) {
		angY2 -= 0.1;
	}
	if (key == 121) {
		angZ2-= 0.1;
	}
	printf("x = %.2f y = %.2f z = %.2f\n", angX,angY,angZ);
	printf("x = %.2f y = %.2f z = %.2f\n\n", angX2, angY2, angZ2);
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("OpenGL - 3D Template");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutKeyboardFunc(key);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 500 / 500, 0.01f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	
	angX = 7;
	angY = 7;
	angZ = 0;
	angX2 = 0;
	angY2 = 0;
	angZ2 = 0.01f;
	glutMainLoop();
}
