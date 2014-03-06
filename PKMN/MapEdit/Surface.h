#ifndef MAP_SURFACE_H
#define MAP_SURFACE_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <functional>
#include <vector>
#include <Misc/SDLImage.h>
#include <Misc/Logger.h>
#include <cstdint>

typedef std::uint32_t button_t;
namespace Key
{
	enum
	{
		LEFT = 1,
		MIDDLE = 2,
		RIGHT = 4,
		SHIFT = 8,
		CTRL = 16,
		ALT = 32,
		WIN = 64,
		FN = 128
	};
}

class Button
{
	public:
		typedef std::function<void(button_t, int, int)> button_fn_t;
		Button(int _x, int _y, int _w, int _h, button_fn_t _fn)
			: x1(_x), y1(_y), x2(_x + _w), y2(_y + _h), fn(_fn)
		{
		}
		bool hit(int x, int y) { return x >= x1 && x < x2 && y >= y1 && y < y2; }
		void operator()(button_t b, int x, int y) { fn(b, x - x1, y - y1); }
	private:
		const int x1, y1, x2, y2;
		button_fn_t fn;
};

class Surface
{
	public:
		Surface(int, int, int, int);
		virtual ~Surface( );
		void render( );
		void click(button_t, int, int);
		bool lock(int, int);
		void unlock( ) { b_lock = -1; }
		const int x, y, width, height;
		static void init(SDL_Renderer *);
		static void freeResources( );
	protected:
		virtual void draw(SDLImage&, int, int) = 0;
		virtual void onClick(button_t, int, int) = 0;
		virtual std::pair<int, int> getScrollSize( ) = 0;
		void addButton(int, int, int, int, Button::button_fn_t);
		void addQuitButton( );
		void quit( );
		static SDL_Renderer *getRenderer( ) { return renderer; }
	private:
		std::uint32_t sysButtons;
		int scroll_x, scroll_y, b_lock;
		SDLImage client, title, scroll_down, scroll_right;
		SDL_Rect title_r, scroll_down_r, scroll_right_r;
		std::vector<Button> buttons;
		void createScrolls( );

		static SDL_Renderer *renderer;
		static SDLImage *border_art;
		static const int SB_QUIT = 1;
};

#endif

