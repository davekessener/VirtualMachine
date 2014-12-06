#ifndef DAV_PKMN_V3_SCREEN_SURFACE
#define DAV_PKMN_V3_SCREEN_SURFACE

#include <memory>
#include "common.h"
#include "sdl.h"
#include "OrderedPair.hpp"
#include "TextEngine.h"

#define MXT_DEFCHARSIZE 8
#define MXT_CHARSETPATH "charset.png"

namespace dav
{
	namespace pkmn
	{
		namespace screen
		{
			class Surface : public std::enable_shared_from_this<Surface>
			{
				public:
				typedef std::shared_ptr<Surface> Surface_ptr;
				typedef std::weak_ptr<Surface> Surface_wptr;
//				typedef OrderedPair<uint, Surface_ptr> child_t;
				typedef Surface_ptr child_t;
				typedef std::vector<child_t> vec_t;
				typedef sdl::MouseButtons MouseButtons;
				typedef sdl::Controls Controls;

				public:
					Surface( ) 
						: id_(nextID()), x_(0), y_(0), w_(0), h_(0), dirty_(true), active_(false) { }
					Surface(const Surface&) = delete;
					Surface& operator=(const Surface&) = delete;
				public:
					void init(int, int, int, int);
					void move(int, int);
					void resize(int, int);
					void update(uint);
					void render( );
					void mouseClick(MouseButtons, int, int, bool);
					void mouseDrag(int, int);
					void mouseHover(int, int);
					void mouseScroll(int);
					void input(Controls, bool);
					Surface_ptr getControlAt(int, int);
					template<typename T> void addChild(std::shared_ptr<T> p, uint v = 0)
						{ addChild(std::dynamic_pointer_cast<Surface>(p), v); }
					void addChild(Surface_ptr, uint = 0);
					void removeChild(DWORD);
					bool hasChild(DWORD) const;
					void fillRect(int, int, int, int, color_t);
					void draw(DWORD, float, float, float, float, int, int, int, int);
					void drawString(const std::string&, int, int, color_t = 0, uint = MXT_DEFCHARSIZE);
				public:
					bool dirty(bool = false);
					void invalidate( );
					void active(bool);
					bool active( ) const { return active_; }
					void setParent(Surface_ptr p) { parent_ = p; updateAP(); }
					bool hasParent( ) const { return static_cast<bool>(parent_.lock()); }
					Surface_ptr parent( )
						{ if(!hasParent()) throw std::string("ERR: No parent!"); return parent_.lock(); }
					coords getPos( ) const { return coords(x_, y_); }
					int X( ) const { return x_; }
					int Y( ) const { return y_; }
					size getSize( ) const { return size(w_, h_); }
					int width( ) const { return w_; }
					int height( ) const { return h_; }
					DWORD ID( ) const { return id_; }
					bbox getAbsoluteAABB( );
					void updateAP( );
				private:
					virtual void i_doInit( ) { };
					virtual void i_doResize( ) { };
					virtual void i_doUpdate(uint) { };
					virtual void i_doPrerender( ) { };
					virtual void i_doRender( ) = 0;
					virtual void i_doMouseDown(MouseButtons, int, int) { }
					virtual void i_doMouseUp(MouseButtons, int, int) { }
					virtual void i_doMouseDrag(int, int) { }
					virtual void i_doMouseHover(int, int) { }
					virtual void i_doMouseScroll(int) { }
					virtual void i_doKeyDown(Controls) { }
					virtual void i_doKeyUp(Controls) { }
				private:
					static DWORD nextID( )
						{ static DWORD nid(0); return ++nid; }
					static DWORD charsetID( )
						{ static DWORD id(0); if(!id) id = TextureEngine::LoadTexture(MXT_CHARSETPATH); return id; }
					static coords mousePos(int nx = -1, int ny = -1)
						{ static uint x(0), y(0); if(nx >= 0) { x = nx; y = ny; } return coords(x, y); }
				private:
					const DWORD id_;
					int x_, y_, w_, h_, dx_, dy_;
					Surface_wptr parent_;
					vec_t children_;
					bool dirty_, active_;
			};

			typedef Surface::Surface_ptr Surface_ptr;
		}
	}
}

#endif

