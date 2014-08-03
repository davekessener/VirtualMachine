#include <SDL2/SDL_opengl.h>
#include <cmath>
#include "gl.h"

namespace gl
{
	void d_gluPerspective(GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar);

	DWORD create_texture(const BYTE *data, int w, int h)
	{
    	GLuint object;

		glGenTextures(1, &object);
    	glBindTexture(GL_TEXTURE_2D, object);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		set_texture(0, data, w, h);
    	
		return object;
	}

	void set_texture(DWORD id, const BYTE *data, int w, int h)
	{
    	if(id) glBindTexture(GL_TEXTURE_2D, id);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	void init2d(int w, int h)
	{
    	glClearColor(0.0, 0.0, 0.0, 0.0);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	glOrtho(0.0, w, h, 1.0, -1.0, 1.0);
    	glEnable(GL_BLEND);
    	glEnable(GL_TEXTURE_2D);
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void init3d(int w, int h, float fov, float znear, float zfar)
	{
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glViewport(0, 0, (GLsizei) w, (GLsizei) h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		d_gluPerspective(fov, (GLfloat) w/(GLfloat) h, znear, zfar);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_CULL_FACE);
	}

	void start_draw(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void push(void)
	{
		glPushMatrix();
	}

	void pop(void)
	{
		glPopMatrix();
	}

	void rotate(float a, float dx, float dy, float dz)
	{
		glRotatef(a, dx, dy, dz);
	}

	void translate(float dx, float dy, float dz)
	{
		glTranslatef(dx, dy, dz);
	}

	void bind_texture(DWORD id)
	{
    	glBindTexture(GL_TEXTURE_2D, id);
	}

	void color(float r, float g, float b)
	{
		glColor3f(r, g, b);
	}

	void draw_face2d(float sx1, float sy1, float sx2, float sy2, int tx1, int ty1, int tx2, int ty2)
	{
    	glBegin(GL_QUADS);
    	    glTexCoord2f(sx1, sy1);
    	    glVertex2f  (tx1, ty1);
    	    glTexCoord2f(sx2, sy1);
    	    glVertex2f  (tx2, ty1);
    	    glTexCoord2f(sx2, sy2);
    	    glVertex2f  (tx2, ty2);
    	    glTexCoord2f(sx1, sy2);
    	    glVertex2f  (tx1, ty2);
    	glEnd();
	}

	void draw_face3d(float sx1, float sy1, float sx2, float sy2, 
		float tx1, float ty1, float tz1, 
		float tx2, float ty2, float tz2,
		float tx3, float ty3, float tz3,
		float tx4, float ty4, float tz4)
	{
    	glBegin(GL_QUADS);
    	    glTexCoord2f(sx1, sy1);
    	    glVertex3f  (tx1, ty1, tz1);
    	    glTexCoord2f(sx2, sy1);
    	    glVertex3f  (tx2, ty2, tz2);
    	    glTexCoord2f(sx2, sy2);
    	    glVertex3f  (tx3, ty3, tz3);
    	    glTexCoord2f(sx1, sy2);
    	    glVertex3f  (tx4, ty4, tz4);
    	glEnd();
	}

	void begin(void)
	{
		glBegin(GL_QUADS);
	}

	void end(void)
	{
		glEnd();
	}

	void addVertex(float x, float y, float z)
	{
		glVertex3f(x, y, z);
	}

	void addUV(float u, float v)
	{
		glTexCoord2f(u, v);
	}

	void update(void)
	{
    	glFlush();
	}

	void delete_texture(DWORD id)
	{
		glDeleteTextures(1, &id);
	}

	void d_gluPerspective(GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar)
	{
		static const GLfloat pi = 3.1415926535897932384626433832795;
		GLfloat fH(tan(fovY * pi / 360.0) * zNear);
		GLfloat fW(fH * aspect);
		glFrustum(-fW, fW, -fH, fH, zNear, zFar);
	}
}

