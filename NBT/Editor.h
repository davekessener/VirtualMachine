#ifndef EDITOR_H
#define EDITOR_H

#include <functional>
#include "CommandableScene.h"

#ifdef EDITOR_MAIN
#endif

class Editor : public CommandableScene
{
	public:
		Editor( );
		~Editor( );
	private:
		void registerCommands( );
		void init( );
		void suspend( );
		void wake( );
		void input(int);
		void update(int);
		void refresh( );
		void finalize( );
};

#endif

