#ifndef PKMN_EDITOR_SURFACE_H
#define PKMN_EDITOR_SURFACE_H

#include "../common.h"
#include <memory>
#include <cassert>
#include <dav/sdl.h>

namespace editor
{
	class Surface : public std::enable_shared_from_this<Surface>
	{
		public:
		typedef std::shared_ptr<Surface> Surface_ptr;
		typedef std::weak_ptr<Surface> Surface_wptr;
		typedef std::vector<Surface_ptr> vec_t;
		typedef dav::sdl::Controls Controls;

		public:
			Surface(uint x, uint y, uint w, uint h)
				: id_(GetID()), x1_(x), y1_(y), x2_(x + w), y2_(y + h), dirty_(false) { }
			inline uint ID( ) const { return id_; }
			inline void setParent(Surface_ptr p) { parent_ = p; }
			inline bool hasParent( ) const { return static_cast<bool>(parent_.lock()); }
			inline Surface_ptr parent( ) { assert(hasParent()); return parent_.lock(); }
			inline void addChild(Surface_ptr p) { children_.push_back(p); p->setParent(shared_from_this()); }
			inline void update(int d) { i_doUpdate(d); for(auto& p : children_) p->update(d); prerender(); }
			inline void prerender( ) { if(dirty_) { i_doPrerender(); dirty_ = false; } }
			inline void render( ) const { i_doRender(); for(const auto& p : children_) p->render(); }
			void keyDown(Controls);
			void keyUp(Controls);
			void mouseDown(Controls, int, int);
			void mouseMove(int, int);
			void mouseUp(Controls, int, int);
			inline bool isDirty( ) const
				{ for(const auto& p : children_) if(p->isDirty()) return true; return dirty_; }
			inline void dirty( ) { dirty_ = true; }
			inline void focus( ) { Focus(id_); }
			inline bool hasFocus( ) const
				{ for(const auto& p : children_) if(p->hasFocus()) return true; return id_ == Focus(); }
		private:
			virtual void i_doUpdate(int) = 0;
			virtual void i_doPrerender( ) { }
			virtual void i_doRender( ) const = 0;
			virtual void i_doKeyDown(Controls) { }
			virtual void i_doKeyUp(Controls) { }
			virtual void i_doMouseDown(Controls, int, int) { }
			virtual void i_doMouseMove(int, int) { }
			virtual void i_doMouseUp(Controls, int, int) { }
		private:
			uint id_;
			int x1_, y1_, x2_, y2_;
			Surface_wptr parent_;
			vec_t children_;
			bool dirty_;
			Surface_ptr mlock_;
		private:
			static uint GetID( ) { static uint id(0); return ++id; }
			static uint Focus(uint i = 0) { static uint f(0); if(i) f = i; return f; }
			static Surface_ptr KeyLock(Surface_ptr = Surface_ptr());
	};

	typedef Surface::Surface_ptr Surface_ptr;
}

#endif

