#ifndef PKMN_EDITOR_SURFACE_H
#define PKMN_EDITOR_SURFACE_H

#include "../../common.h"
#include <deque>
#include <memory>
#include <algorithm>
#include <cassert>
#include <dav/sdl.h>

namespace editor
{
	namespace surface
	{
		class Surface : public std::enable_shared_from_this<Surface>
		{
			public:
			typedef std::shared_ptr<Surface> Surface_ptr;
			typedef std::weak_ptr<Surface> Surface_wptr;
			typedef std::deque<Surface_ptr> vec_t;
			typedef dav::sdl::Controls Controls;
			typedef dav::sdl::MouseButtons MouseButtons;

			public:
				Surface( ) : id_(GetID()), dirty_(true), hidden_(false) { }
				inline void init(Surface_ptr p = Surface_ptr()) { if(p) parent_ = p; i_doInit(); }
				inline void init(uint x, uint y, uint w, uint h, Surface_ptr p = Surface_ptr())
					{ if(p) parent_ = p; x1_ = x; y1_ = y; x2_ = x + w; y2_ = y + h; i_doInit(); }
				inline uint ID( ) const { return id_; }
				inline void setParent(Surface_ptr p) { parent_ = p; }
				inline bool hasParent( ) const { return static_cast<bool>(parent_.lock()); }
				inline Surface_ptr parent( ) { assert(hasParent()); return parent_.lock(); }
				inline void addChild(Surface_ptr p) { children_.push_front(p); p->setParent(shared_from_this()); }
				inline void removeChild(uint id)
				{ 
					children_.erase(
						std::find_if(
							children_.begin(),
							children_.end(),
							[id](Surface_ptr p)
							{ 
								return p->id_ == id;
							}
						)
					);
				}
				inline void update(int d) { i_doUpdate(d); for(auto& p : children_) p->update(d); prerender(); }
				inline void prerender( ) { if(dirty_) { i_doPrerender(); dirty_ = false; } }
				void render( ) const;
				void keyDown(Controls);
				void keyUp(Controls);
				void mouseDown(MouseButtons, int, int);
				void mouseMove(int, int);
				void mouseUp(MouseButtons, int, int);
				void scroll(int);
				inline bool isDirty( ) const
					{ for(const auto& p : children_) if(p->isDirty()) return true; return dirty_; }
				inline void dirty( ) { dirty_ = true; }
				inline void focus( ) { Focus(id_); }
				inline bool hasFocus( ) const
					{ for(const auto& p : children_) if(p->hasFocus()) return true; return id_ == Focus(); }
				inline bool hasWFocus( ) const
					{ for(const auto& p : children_) if(p->hasWFocus()) return true; return id_ == WFocus(); }
				inline uint width( ) const { return x2_ - x1_; }
				inline uint height( ) const { return y2_ - y1_; }
				inline void hide( ) { i_doHide(); hidden_ = true; }
				inline void show( ) { i_doShow(); hidden_ = false; }
				inline bool hidden( ) const { return hidden_; }
			protected:
				point getAbsCoords( ) const;
			private:
				virtual void i_doInit( ) = 0;
				virtual void i_doUpdate(int) = 0;
				virtual void i_doPrerender( ) { }
				virtual void i_doRender( ) const = 0;
				virtual void i_doKeyDown(Controls) { }
				virtual void i_doKeyUp(Controls) { }
				virtual void i_doMouseDown(MouseButtons, int, int) { }
				virtual void i_doMouseMove(int, int) { }
				virtual void i_doMouseUp(MouseButtons, int, int) { }
				virtual void i_doScroll(int) { }
				virtual void i_doHide( ) { }
				virtual void i_doShow( ) { }
			private:
				uint id_;
				int x1_, y1_, x2_, y2_;
				Surface_wptr parent_;
				vec_t children_;
				bool dirty_, hidden_;
				Surface_ptr mlock_;
			private:
				static uint GetID( ) { static uint id(0); return ++id; }
				static Surface_ptr KeyLock(Surface_ptr = Surface_ptr());
			public:
				static uint Focus(uint i = 0) { static uint f(0); if(i) f = i; return f; }
				static uint WFocus(uint i = 0) { static uint f(0); if(i) f = i; return f; }
		};

		typedef Surface::Surface_ptr Surface_ptr;
	}
}

#endif

