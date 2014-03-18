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
			Scrolling(Image *, int, int, int, int, std::function<T *(Image *, int, int, int, int)> = std::function<T *(Image *, int, int, int, int)>{});
			~Scrolling( );
			void mouseDown(button_t, int, int);
			void mouseDrag(button_t, int, int);
			void mouseUp(button_t, int, int);
			T& getContent( );
		protected:
			void draw(Image *);
			bool isDirty( );
		private:
			ScrollBar *vscroll, *hscroll;
			Scrollable *content;
	};

// # ---------------------------------------------------------------------------

	template<typename T>
	Scrolling<T>::Scrolling(Image *i, int x, int y, int w, int h, std::function<T *(Image *, int, int, int, int)> mc)
		: Surface(i = new SubImage(i, x, y, w, h))
	{
		content = static_cast<bool>(mc) ? mc(i, 0, 0, w - TILE_SIZE, h - TILE_SIZE) : new T(i, 0, 0, w - TILE_SIZE, h - TILE_SIZE);
		std::pair<int, int> ranges = content->getScrollRanges();
		registerSurface(hscroll = new HScrollBar(i, 0, h - TILE_SIZE, w - TILE_SIZE, ranges.first));
		registerSurface(vscroll = new VScrollBar(i, w - TILE_SIZE, 0, h - TILE_SIZE, ranges.second));
	}
	
	template<typename T>
	Scrolling<T>::~Scrolling(void)
	{
		delete content;
		delete hscroll;
		delete vscroll;
	}

	template<typename T>
	bool Scrolling<T>::isDirty(void)
	{
		return Surface::isDirty() || content->isDirty();
	}

	template<typename T>
	void Scrolling<T>::mouseDown(button_t b, int x, int y)
	{
		content->mouseDown(b, x + hscroll->getRelativePosition(), y + vscroll->getRelativePosition());
	}

	template<typename T>
	void Scrolling<T>::mouseDrag(button_t b, int x, int y)
	{
		content->mouseDrag(b, x + hscroll->getRelativePosition(), y + vscroll->getRelativePosition());
	}

	template<typename T>
	void Scrolling<T>::mouseUp(button_t b, int x, int y)
	{
		content->mouseUp(b, x + hscroll->getRelativePosition(), y + vscroll->getRelativePosition());
	}
	
	template<typename T>
	void Scrolling<T>::draw(Image *dI)
	{
		content->draw(content->getDrawSurface(), hscroll->getRelativePosition(), vscroll->getRelativePosition());
	}

	template<typename T>
	T& Scrolling<T>::getContent(void)
	{
		return *dynamic_cast<T *>(content);
	}
}

#endif

