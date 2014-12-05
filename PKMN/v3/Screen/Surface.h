#ifndef DAV_PKMN_V3_SCREEN_SURFACE
#define DAV_PKMN_V3_SCREEN_SURFACE

#include <memory>
#include "common.h"
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
				typedef OrderedPair<uint, Surface_ptr> child_t;
				typedef std::vector<child_t> vec_t;

				public:
					Surface( ) : id_(nextID()), x_(0), y_(0), w_(0), h_(0), dirty_(true) { }
					Surface(const Surface&) = delete;
					Surface& operator=(const Surface&) = delete;
				public:
					void init(int, int, uint, uint);
					void move(int, int);
					void resize(uint, uint);
					void update(uint);
					void render( );
					void addChild(Surface_ptr, uint = 0);
					void removeChild(DWORD);
					bool hasChild(DWORD) const;
					void fillRect(int, int, int, int, color_t);
					void draw(DWORD, float, float, float, float, int, int, int, int);
					void drawString(const std::string&, int, int, color_t = 0, uint = MXT_DEFCHARSIZE);
				public:
					bool dirty(bool = false);
					void invalidate( );
					void setParent(Surface_ptr p) { parent_ = p; }
					bool hasParent( ) const { return static_cast<bool>(parent_.lock()); }
					Surface_ptr parent( )
						{ if(!hasParent()) throw std::string("ERR: No parent!"); return parent_.lock(); }
					coords getPos( ) const { return coords(x_, y_); }
					size getSize( ) const { return size(w_, h_); }
					DWORD ID( ) const { return id_; }
					bbox getAbsoluteAABB( );
					coords getAbsolutePosition( );
				private:
					virtual void i_doInit( ) = 0;
					virtual void i_doResize( ) = 0;
					virtual void i_doUpdate(uint) = 0;
					virtual void i_doRender( ) = 0;
				private:
					static DWORD nextID( )
						{ static DWORD nid(0); return ++nid; }
					static DWORD charsetID( )
						{ static DWORD id(0); if(!id) id = TextureEngine::LoadTexture(MXT_CHARSETPATH); return id; }
				private:
					const DWORD id_;
					int x_, y_;
					int w_, h_;
					Surface_wptr parent_;
					vec_t children_;
					bool dirty_;
			};

			typedef Surface::Surface_ptr Surface_ptr;
		}
	}
}

#endif

