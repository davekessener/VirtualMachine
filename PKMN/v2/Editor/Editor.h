#ifndef PKMN_EDITOR_H
#define PKMN_EDITOR_H

#include "../common.h"
#include <string>
#include <vector>
#include <set>
#include <dav/sdl.h>
#include "surface/Surface.h"

namespace editor
{
	class Editor
	{
		public:
		typedef std::vector<std::string> params_t;
		typedef dav::sdl::Controls Controls;
		typedef dav::sdl::MouseButtons MouseButtons;

		public:
			Editor(uint, uint);
			void init( );
			void update(int);
			void keyboard(Controls, bool);
			void mouseMove(uint, uint, int, int);
			void mouseClick(MouseButtons, uint, uint, bool);
			void mouseWheel(int, int);
			static int run(const params_t&);
		private:
			void quit( ) { running_ = false; }
		private:
			uint width_, height_;
			bool running_;
			surface::Surface_ptr root_, dialog_;

			struct Keys
			{
				public:
					inline void press(Controls k, bool p)
					{
						if(p) keys_.insert(k);
						else { auto i = keys_.find(k); if(i != keys_.end()) keys_.erase(i); }
					}
					inline bool isPressed(Controls k) const { return keys_.find(k) != keys_.cend(); }
				private:
					std::set<Controls> keys_;
			} modifier_;
	};
}

#endif

