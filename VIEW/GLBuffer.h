#ifndef GLBUFFER_H
#define GLBUFFER_H

#include <map>
#include "inc.h"

class GLBuffer
{
	public:
		GLBuffer( );
		~GLBuffer( ) noexcept;
		void set(const BYTE *, int, int);
		void render(float, float, float, float, int, int, int, int) const;
		void bind( ) const;
	private:
		bool hasTexture(int) const;
	private:
		DWORD id_;
		int w_, h_, pp_;
		float rw_, rh_;
		std::map<int, DWORD> tex_;
};

#endif

