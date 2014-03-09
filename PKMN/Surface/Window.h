#ifndef _WINDOW_H
#define _WINDOW_H

#include "StatusBar.h"
#include "Scrollable.h"
#include <SDL_ttf.h>

namespace surface
{
	template<typename T>
	class Window : public Surface
	{
		public:
			Window(Image *, const std::string&);
			virtual ~Window( );
			static const int BORDER_SIZE = 4;
		protected:
			void draw(Image *);
		private:
			Image *createBg( );
			StatusBar *status;
			T *content;
			std::string title;
			Image *bg;
	};

	template<typename T>
	Window<T>::Window(Image *i, const std::string& t) 
		: Surface(new SubImage(i, 0, 0, i->width(), i->height())), title(t)
	{
		registerSurface(status = new StatusBar(i, BORDER_SIZE, BORDER_SIZE, i->width() - 2 * BORDER_SIZE, title));
		registerSurface(content = new T(i, BORDER_SIZE, BORDER_SIZE + StatusBar::BAR_SIZE + 2, W() - 2 * BORDER_SIZE, H() - 2 * BORDER_SIZE - StatusBar::BAR_SIZE - 2));
		bg = createBg();
	}

	template<typename T>
	Window<T>::~Window(void)
	{
		delete bg;
		delete content;
		delete status;
	}

	template<typename T>
	void Window<T>::draw(Image *dI)
	{
		dI->blit(bg, Point(0, 0), Rect(0, 0, bg->width(), bg->height()));
	}

	template<typename T>
	Image *Window<T>::createBg(void)
	{
		Image *i = new Image(W(), H());
		int w = i->width(), h = i->height();

		const unsigned int COLOR[] = {0xffffffff, 0xd4d0c8ff, 0x808080ff, 0x404040ff, 0x000000ff};

		i->startBlit();

		i->fillRect(COLOR[1], 0, 0, w, h);
		i->fillRect(COLOR[0], 4, 26, w - 8, h - 30);
		i->drawLine(COLOR[0], 1, 1, w - 3, 1);
		i->drawLine(COLOR[0], 1, 1, 1, h - 3);
		i->drawLine(COLOR[2], 1, h - 2, w - 2, 1);
		i->drawLine(COLOR[2], w - 2, 1, 1, h - 2);
		i->drawLine(COLOR[3], 0, h - 1, w, 1);
		i->drawLine(COLOR[3], w - 1, 0, 1, h);

		i->endBlit();

		return i;
	}
}

#endif

