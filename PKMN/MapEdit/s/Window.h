#ifndef _WINDOW_H
#define _WINDOW_H

#include "statusBar.h"
#include "Scrollable.h"

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
			StatusBar *status;
			Scrolling<T> *scroll;
			std::string title;
	};

	template<typename T>
	Window<T>::Window(Image *i, const std::string& t) 
		: Surface(new SubImage(i, 0, 0, i->width(), i->height()), 0, 0), title(t)
	{
		registerSurface(status = new StatusBar(i, BORDER_SIZE, BORDR_SIZE, i->width() - 2 * BORDER_SIZE, title));
		registerSurface(scroll = new Scrolling<T>(i, BORDER_SIZE, BORDER_SIZE + StatusBar::BAR_SIZE + 2, W() - 2 * BORDER_SIZE, H() - 2 * BORDER_SIZE - StatusBar::BAR_SIZE - 2));
	}

	template<typename T>
	Window<T>::~Window(void)
	{
		delete scroll;
		delete status;
	}

	template<typename T>
	void Window<T>::draw(Image *dI)
	{
		dI->startBlit();
		// draw window border
		dI->endBlit();
	}
}

#endif

