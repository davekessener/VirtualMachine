#ifndef LIB_GL_H
#define LIB_GL_H

#include "inc.h"

namespace gl
{
	DWORD create_texture(const BYTE *, int, int);
	void init2d(int, int);
	void init3d(int, int);
	void start_draw( );
	void push( );
	void pop( );
	void rotate(float, float, float, float);
	void translate(float, float, float);
	void bind_texture(DWORD);
	void color(float, float, float);
	void draw_face2d(float, float, float, float, int, int, int, int);
	void draw_face3d(float, float, float, float,
		float, float, float, 
		float, float, float, 
		float, float, float, 
		float, float, float);
	void update( );
	void delete_texture(DWORD);
}

#endif

