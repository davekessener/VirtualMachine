#ifndef TESSELLATOR_H
#define TESSELLATOR_H

#include <array>
#include "inc.h"
#include "gl.h"

#define MXT_TESS_BUFSIZE 0x200000

class Tessellator
{
	typedef union { std::uint32_t i; float f; } data_t;

	public:
		static Tessellator& instance( );
		void startDrawing( );
		void draw( );
		void addVertex(float, float, float);
		void addUV(float, float);
		void addVertexUV(float x, float y, float z, float u, float v) { addUV(u, v); addVertex(x, y, z); }
		inline bool isDrawing( ) const { return drawing_; }
		inline void setOffset(float dx, float dy, float dz) { dx_ = dx; dy_ = dy; dz_ = dz; }
		inline void addOffset(float dx, float dy, float dz) { dx_ += dx; dy_ += dy; dz_ += dz; }
	private:
		void reset( );
	private:
		bool drawing_, hasTextures_;
		int count_, idx_;
		float u_, v_, dx_, dy_, dz_;
		std::array<data_t, MXT_TESS_BUFSIZE> buf_;
	
	private:
		Tessellator( );
		Tessellator(const Tessellator&);
		~Tessellator( ) noexcept;
		Tessellator& operator=(const Tessellator&);
};

#endif

