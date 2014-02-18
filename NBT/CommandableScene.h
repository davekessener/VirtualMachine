#ifndef CMDSCENE_H
#define CMDSCENE_H

#include <iostream>
#include <sstream>
#include <functional>
#include <map>
#include <vector>
#include <cstring>
#include "Terminal.h"
#include "Scene.h"

#ifdef CMDSCENE_MAIN
#define STD_BUFSIZE 256
#endif

class CommandableScene : public Scene
{
	typedef std::function<void(std::vector<std::string>)> commandFn_t;

	public:
		CommandableScene( );
		~CommandableScene( );
	protected:
		void init( );
		void refresh( );
		virtual void registerCommands( ) = 0;
		void registerCommand(const std::string&, commandFn_t);
		void assignKeys(const std::string&, std::initializer_list<int>);
	private:
		void startInput( );
		bool inputHandler(int);
		bool processInput(int);
		void resize( );
		bool isSuitable(int);
		std::vector<std::string> processCommand(const std::string&);
		void command(const std::string&);
		std::map<int, std::string> keymap;
		std::map<std::string, commandFn_t> cmdmap;
		char *_buf;
		int _i, _l, _idx;
};

#endif

