#ifndef LIB_GL_H
#define LIB_GL_H

#include "inc.h"

namespace gl
{
	enum
	{
		RGB,
		RGBA,
		LINEAR,
		NEAR
	};

	DWORD create_texture(const void *, int, int, int = RGB, int = NEAR);
	void set_texture(DWORD, const void *, int, int, int = RGB);
	void init2d(int, int);
	void init3d(int, int, float = 45.0, float = 1.0, float = 100.0);
	void start_draw( );
	void push( );
	void pop( );
	void begin();
	void end();
	void rotate(float, float, float, float);
	void translate(float, float, float);
	void bind_texture(DWORD);
	void color(DWORD);
	void color(float, float, float);
	void draw_rect(int, int, int, int, int = -1);
	void draw_face2d(float, float, float, float, int, int, int, int);
	void draw_face3d(float, float, float, float,
		float, float, float, 
		float, float, float, 
		float, float, float, 
		float, float, float);
	void addVertex(float, float, float);
	void addUV(float, float);
	inline void addVertexUV(float x, float y, float z, float u, float v) { addUV(u, v); addVertex(x, y, z); }
	void update( );
	void delete_texture(DWORD);
}

#endif

