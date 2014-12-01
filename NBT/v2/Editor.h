#ifndef EDITOR_H
#define EDITOR_H

#include "CommandableScene.h"

class Editor : public CommandableScene
{
	using CommandableScene::params_t;

	public:
		Editor( );
		Editor(const std::vector<std::string>&);
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
		struct Impl;
		Impl *impl_;
};

#endif

