#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define MXT_WWIDTH 600
#define MXT_WHEIGHT 800

void d_gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

int d_main(int argc, char *argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *win(SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MXT_WHEIGHT, MXT_WWIDTH, SDL_WINDOW_OPENGL));
	SDL_GL_CreateContext(win);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat ratio((GLfloat)MXT_WHEIGHT / (GLfloat)MXT_WWIDTH);
	glViewport(0, 0, (GLsizei)MXT_WHEIGHT, (GLsizei)MXT_WWIDTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	d_gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -6.0f);

	for(float a = 0.0f ; a < 360.0f ; a += 0.5f)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();

		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(0.0f, 0.0f, -5.0 + 2.5 * sin(a * 3.141f / 180.0f));
		glRotatef(a, 0.0f, 1.0f, 0.0f);
		
		for(int i = 0 ; i < 3 ; ++i)
		{
			glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
			glColor3f(i == 0, i == 1, i == 2);
			glBegin(GL_TRIANGLES);
			  glVertex3f(-0.5f,  0.0f, -0.29f);
			  glVertex3f( 0.5f,  0.0f, -0.29f);
			  glVertex3f( 0.0f,  0.866f, 0.0f);
			glEnd();
		}

		glPopMatrix();
		
		SDL_GL_SwapWindow(win);
		SDL_Delay(5);
	}

	SDL_Quit();

	return 0;
}

void d_gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	static const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fH(tan(fovY * pi / 360.0) * zNear);
	GLdouble fW(fH * aspect);
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

