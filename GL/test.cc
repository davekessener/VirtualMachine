#include <cstdio>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

extern void d_gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

static int shoulder = 0, elbow = 0, finger = 0;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void renderCube(GLfloat f = 1.0f, unsigned long c = 0xffffff)
{
#define MXT_RED(c) (((c >> 16) & 0xff) / 255.0)
#define MXT_GREEN(c) (((c >> 8) & 0xff) / 255.0)
#define MXT_BLUE(c) (( c & 0xff) / 255.0)
	glColor3f(MXT_RED(c), MXT_GREEN(c), MXT_BLUE(c));
	glBegin(GL_QUADS);
		glVertex3f(-0.5 * f, -0.5 * f, -0.5 * f);
		glVertex3f( 0.5 * f, -0.5 * f, -0.5 * f);
		glVertex3f( 0.5 * f,  0.5 * f, -0.5 * f);
		glVertex3f(-0.5 * f,  0.5 * f, -0.5 * f);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(-0.5 * f, -0.5 * f,  0.5 * f);
		glVertex3f( 0.5 * f, -0.5 * f,  0.5 * f);
		glVertex3f( 0.5 * f,  0.5 * f,  0.5 * f);
		glVertex3f(-0.5 * f,  0.5 * f,  0.5 * f);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(-0.5 * f, -0.5 * f, -0.5 * f);
		glVertex3f(-0.5 * f, -0.5 * f,  0.5 * f);
		glVertex3f(-0.5 * f,  0.5 * f,  0.5 * f);
		glVertex3f(-0.5 * f,  0.5 * f, -0.5 * f);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f( 0.5 * f, -0.5 * f, -0.5 * f);
		glVertex3f( 0.5 * f, -0.5 * f,  0.5 * f);
		glVertex3f( 0.5 * f,  0.5 * f,  0.5 * f);
		glVertex3f( 0.5 * f,  0.5 * f, -0.5 * f);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(-0.5 * f,  0.5 * f, -0.5 * f);
		glVertex3f( 0.5 * f,  0.5 * f, -0.5 * f);
		glVertex3f( 0.5 * f,  0.5 * f,  0.5 * f);
		glVertex3f(-0.5 * f,  0.5 * f,  0.5 * f);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(-0.5 * f, -0.5 * f, -0.5 * f);
		glVertex3f( 0.5 * f, -0.5 * f, -0.5 * f);
		glVertex3f( 0.5 * f, -0.5 * f,  0.5 * f);
		glVertex3f(-0.5 * f, -0.5 * f,  0.5 * f);
	glEnd();
	glScalef(1.01, 1.01, 1.01);
	glColor3f(1.0 - MXT_RED(c), 1.0 - MXT_GREEN(c), 1.0 - MXT_BLUE(c));
	glutWireCube(f);
#undef MXT_RED
#undef MXT_GREEN
#undef MXT_BLUE
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
		glRotatef((GLfloat) shoulder, 1.0, 0.0, 0.0);
		glRotatef((GLfloat) elbow,    0.0, 1.0, 0.0);
		glRotatef((GLfloat) finger,   0.0, 0.0, 1.0);
		renderCube(1.0, 0xff0000);
	glPopMatrix();

//	glPushMatrix();
//		glTranslatef(-1.0, 0.0, 0.0);
//		glRotatef((GLfloat) shoulder, 0.0, 0.0, 1.0);
//		glTranslatef(1.0, 0.0, 0.0);
//	
//		glPushMatrix();
//			glScalef(2.0, 0.5, 1.0);
//			glutWireCube(1.0);
//		glPopMatrix();
//		
//		glTranslatef(1.0, 0.0, 0.0);
//		glRotatef((GLfloat) elbow, 0.0, 0.0, 1.0);
//		glTranslatef(1.0, 0.0, 0.0);
//		
//		glPushMatrix();
//			glScalef(2.0, 0.5, 1.0);
//			glutWireCube(1.0);
//		glPopMatrix();
//
//		glTranslatef(1.0, 0.0, 0.0);
//		glRotatef((GLfloat) finger, 0.0, 0.0, 1.0);
//		glTranslatef(0.3, 0.0, 0.0);
//		
//		glPushMatrix();
////			glTranslatef(1.6, 0.22, 0.0);
////			glRotatef((GLfloat) finger, 0.0, 0.0, 1.0);
//			glScalef(0.6, 0.1, 0.08);
//			glutWireCube(1.0);
//		glPopMatrix();
//	glPopMatrix();
	
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	d_gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -7.5);
}

void keyboard(unsigned char key, int x, int y) 
{
	switch(key) 
	{
		case 's': /* s key rotates at shoulder */
			shoulder = (shoulder + 5) % 360;
			glutPostRedisplay();
			break;
		case 'S':
			shoulder = (shoulder - 5) % 360;
			glutPostRedisplay();
			break;
		case 'e': /* e key rotates at elbow */
			elbow = (elbow + 5) % 360;
			glutPostRedisplay();
			break;
		case 'E':
			elbow = (elbow - 5) % 360;
			glutPostRedisplay();
			break;
		case 'f': /* f key rotates at finger */
			finger = (finger + 5) % 360;
			glutPostRedisplay();
			break;
		case 'F':
			finger = (finger - 5) % 360;
			glutPostRedisplay();
			break;
		default:
			printf("'%c'(0x%02x): @[%d|%d]\n", key, key, x, y);
			break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	
	return 0;
} 

