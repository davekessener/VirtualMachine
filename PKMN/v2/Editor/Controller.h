#ifndef PKMN_EDITOR_CONTROLLER_H
#define PKMN_EDITOR_CONTROLLER_H

#include "../common.h"
#include "File.h"

namespace editor
{
	class Controller
	{
		typedef File::map_t map_t;

		public:
			static void load(QWORD);
			static bool isLoaded( );
			static const map_t& getMap( );
			template<typename T>
				static void set(uint, uint, uint, const T&);
			static void set(uint, uint, uint, DWORD);
			static DWORD get(uint, uint, uint);
			static void undo( );
			static void redo( );
			static bool hasChanged( );
			static void save( );
		private:
			Controller( ) = default;
			~Controller( );
			void doLoad(QWORD);
			void setBuffer(uint, uint, uint, DWORD);
			void commitBuffer( );
			static Controller& instance( ) { static Controller c; return c; }
		private:
			struct Impl;
			Impl *impl_;
		private:
			Controller(const Controller&) = delete;
			Controller& operator=(const Controller&) = delete;
	};

	template<typename T>
	void Controller::set(uint l, uint x, uint y, const T& v)
	{
		uint w = v.width(), h = v.height();

		for(uint dy = 0 ; dy < h ; ++dy)
		{
			for(uint dx = 0 ; dx < w ; ++dx)
			{
				setBuffer(l, x + dx, y + dy, static_cast<DWORD>(v.at(dx, dy)));
			}
		}

		commitBuffer();
	}
}

#endif

