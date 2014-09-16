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
		typedef surface::Surface_ptr Surface_ptr;

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
			void newFile( );
			void openFile( );
			void saveFile( );
			void saveAs( );
			void closeFile( );
			void quit( );
			bool canSave( );
			bool canSaveAs( );
			bool canClose( );
		private:
			inline void setDialog(Surface_ptr p)
				{ dialog_ = p; p->init(width_ / 3, height_ / 3, width_ / 3, height_ / 3); }
			inline void setFileSelect(Surface_ptr p)
				{ dialog_ = p; p->init(width_ / 4, height_ / 4, width_ / 2, height_ / 2); }
			void setError(const std::string& e);
			void tryToCloseFile(std::function<void(void)>);
			void doQuit( );
		private:
			uint width_, height_;
			bool running_;
			Surface_ptr root_, dialog_, file_;
	};
}

#endif

