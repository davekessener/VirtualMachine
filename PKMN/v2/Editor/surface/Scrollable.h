#ifndef PKMN_EDITOR_SURFACE_SCROLLABLE_H
#define PKMN_EDITOR_SURFACE_SCROLLABLE_H

#include "../../common.h"
#include "Surface.h"

namespace editor
{
	namespace surface
	{
		class Scrollable : public Surface
		{
			public:
				virtual point getScrollSizes( ) const = 0;
				virtual void setScrollPos(int, int) = 0;
			private:
		};

		class ScrollBar : public Surface
		{
			typedef std::function<void(int)> change_fn;
			using Surface::MouseButtons;

			public:
				explicit ScrollBar(change_fn chfn, int g = 0) : barak_(chfn), gran_(g) { }
				inline void setScale(uint i)
					{ if(gran_ == (int)i) return; gran_ = i; if(pos_ > gran_) pos_ = gran_ > 0 ? gran_ - 1 : 0; dirty(); }
				inline void setPos(int i)
					{ if(i >= 0 && i <= gran_ && i != pos_) { pos_ = i; dirty(); barak_(pos_); } }
				inline int getPos( ) const { return pos_; }
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doPrerender( );
				void i_doRender( ) const;
				void i_doMouseDown(MouseButtons, int, int);
				void i_doMouseMove(int, int);
				void i_doMouseUp(MouseButtons, int, int);
				inline int getPosFrom(int x, int y) const { return ((f_ ? y : x) -  s_) / w_; }
			private:
				change_fn barak_;
				point p1_, p2_;
				bool f_, down_;
				int gran_, pos_;
				int s_, w_;
				DWORD mesh_;
		};

		class Scrolling : public Surface
		{
			typedef std::shared_ptr<Scrollable> Scrollable_ptr;
			typedef std::shared_ptr<ScrollBar> ScrollBar_ptr;

			public:
				void setScrollable(Scrollable_ptr p) { content_ = p; }
			private:
				void i_doInit( );
				void i_doUpdate(int);
				void i_doPrerender( );
				void i_doRender( ) const;
			private:
				Scrollable_ptr content_;
				ScrollBar_ptr hor_, ver_;
				point smax_, scur_;
//				DWORD mesh_;
//				int vs_, hs_;
		};
	}
}

#endif

