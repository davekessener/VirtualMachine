#ifndef EDITOR_H
#define EDITOR_H

#include <functional>
#include <memory>
#include "CommandableScene.h"
#include "TreeView.h"
#include "Object.h"

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
	private:
		std::shared_ptr<TreeView> tv_;
		Object_ptr o_;
		struct { int x, y, w, h; } winsize_;
};

#endif

