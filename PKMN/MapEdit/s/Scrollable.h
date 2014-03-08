#ifndef _SCROLLABLE_H
#define _SCROLLABLE_H

#include "Surface.h"
#include "ScrollBar.h"
#include "VScrollBar.h"
#include "HScrollBar.h"

namespace surface
{
	template<typename T>
	class Scrolling;

	class Scrollable : public Surface
	{
		public:
			Scrollable(Image *, int, int, int, int);
			~Scrollable( );
		protected:
			void draw(Image *);
			virtual void draw(Image *, int, int) = 0;
			virtual std::pair<int, int> getScrollRanges( ) = 0;
		private:
			template<typename> friend class Scrolling;
	};

// # ===========================================================================

	template<typename T>
	class Scrolling : public Surface
	{
		public:
			Scrolling(Image *, int, int, int, int);
			~Scrolling( );
		protected:
			void draw(Image *);
		private:
			ScrollBar *vscroll, *hscroll;
			Scrollable *content;
	};

// # ---------------------------------------------------------------------------

	template<typename T>
	Scrolling<T>::Scrolling(Image *i, int x, int y, int w, int h) : Surface(i = new SubImage(i, x, y, w, h))
	{
		content = new T(i, 0, 0, w - TILE_SIZE, h - TILE_SIZE);
		std::pair<int, int> ranges = content->getScrollRanges();
		registerSurface(hscroll = new HScrollBar(i, 0, h - TILE_SIZE, w - TILE_SIZE, ranges.first));
		registerSurface(vscroll = new VScrollBar(i, w - TILE_SIZE, 0, h - TILE_SIZE, ranges.second));
		registerSurface(content);
	}
	
	template<typename T>
	Scrolling<T>::~Scrolling(void)
	{
		delete content;
		delete hscroll;
		delete vscroll;
	}
	
	template<typename T>
	void Scrolling<T>::draw(Image *dI)
	{
		content->draw(dI, hscroll->getRelativePosition(), vscroll->getRelativePosition());
	}
}

#endif

