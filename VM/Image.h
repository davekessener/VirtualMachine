//#ifndef VM_IMAGE_H
//#define VM_IMAGE_H
//
//#include "ddef.h"
//#include <SDL2/SDL.h>
//#include <string>
//
//namespace vm
//{
//	namespace sdl
//	{
//		class color_rgba_t
//		{
//			typedef std::uint8_t d_t;
//		
//			public:
//				color_rgba_t(std::uint32_t v = 0x000000ff) : v_(v) { }
//				color_rgba_t(d_t r, d_t g, d_t b, d_t a = 0xff) { set(r, g, b, a); }
//				void set(d_t r, d_t g, d_t b, d_t a = 0xff) { v_ = (r << 24) | (g << 16) | (b << 8) | a; }
//				d_t r( ) const { return (v_ >> 24) & 0xff; }
//				d_t g( ) const { return (v_ >> 16) & 0xff; }
//				d_t b( ) const { return (v_ >>  8) & 0xff; }
//				d_t a( ) const { return  v_        & 0xff; }
//				operator std::uint32_t( ) const { return v_; }
//				explicit operator SDL_Color( ) const { SDL_Color c = {r(), g(), b(), a()}; return c; }
//				const color_rgba_t invert( ) { return color_rgba_t(0xff - r(), 0xff - g(), 0xff - b(), a()); }
//			private:
//				std::uint32_t v_;
//		};
//
//		class Image
//		{
//			public:
//				static void init(SDL_Renderer *r);
//				Image( );
//				Image(size_t, size_t);
//				Image(const std::string&);
//				~Image( );
//				void create(size_t, size_t);
//				void open(const std::string&);
//				void close( );
//				size_t width( ) const;
//				size_t height( ) const;
//				explicit operator SDL_Texture *( );
//				void fillRect(color_rgba_t, int, int, int, int);
//				void drawChar(color_rgba_t, int, int, char);
//				void clear( );
//				void startBlit( );
//				void stopBlit( );
//				void render( );
//			private:
//				SDL_Texture *tex_;
//				size_t width_, height_;
//				color_rgba_t bg_;
//			private:
//				Image(const Image&);
//				Image& operator=(const Image&);
//
//			private:
//				class tex_holder;
//				static SDL_Renderer *renderer__;
//				static tex_holder charset__;
//		};
//	}
//}
//
//#endif

