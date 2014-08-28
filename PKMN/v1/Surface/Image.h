#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream>
#include <stack>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <cassert>
#include "Geometry.h"
#include "FontManager.h"

class color_rgba_t
{
	public:
		inline color_rgba_t(int _r, int _g, int _b, int _a = 0xff)
		{
			set(_r, _g, _b, _a);
		}
		inline color_rgba_t(unsigned int v = 0xffffffff) : _v(v) { }
		inline void set(int _r, int _g, int _b, int _a)
		{
			_v = ((_r & 0xff) << 24) | ((_g & 0xff) << 16) | ((_b & 0xff) << 8) | (_a & 0xff);
		}
		inline operator unsigned int( ) { return _v; }
		inline explicit operator SDL_Color( ) { SDL_Color c = {r(), g(), b(), a()}; return c; }
		inline unsigned char r( ) { return (_v >> 24) & 0xff; }
		inline unsigned char g( ) { return (_v >> 16) & 0xff; }
		inline unsigned char b( ) { return (_v >>  8) & 0xff; }
		inline unsigned char a( ) { return  _v        & 0xff; }
		inline color_rgba_t inverse( ) { return color_rgba_t((0xffffffff - _v) | a()); }
	private:
		unsigned int _v;
};

class Image
{
	public:
		Image( );
		Image(const std::string&);
		Image(int, int);
		virtual ~Image( );
		void setRenderer(SDL_Renderer *r) { render = r; }
		void create(int, int);
		void open(const std::string&);
		void close( );
		virtual void blit(const Image *, Point, Rect);
		void startBlit( );
		void endBlit( );
		void checkBlit( );
		bool opened( ) { return img != NULL; }
		virtual int X( ) const { return 0; }
		virtual int Y( ) const { return 0; }
		int width( ) const { return _width; }
		int height( ) const { return _height; }
		void gradientFill(color_rgba_t, color_rgba_t, color_rgba_t, color_rgba_t);
		void drawRect(color_rgba_t, int, int, int, int);
		void fillRect(color_rgba_t, int, int, int, int);
		void drawLine(color_rgba_t, int, int , int , int);
		void renderText(const std::string&, int, int, color_rgba_t = 0x000000ff);
		void clear(color_rgba_t, bool = false);
		void drawGrid(color_rgba_t, int, int);
		virtual Image *base( );
		virtual explicit operator SDL_Texture *( ) const;
		static void init(SDL_Renderer *r) { render = r; }
	protected:
		int _width, _height;
	private:
		Image(SDL_Surface *);
		SDL_Texture *img;
		static SDL_Renderer *render;
		static std::stack<SDL_Texture *> blitting;
};

#endif

