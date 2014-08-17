#ifndef CMDSCENE_H
#define CMDSCENE_H

#include <iosfwd>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include "Scene.h"

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
		void assignKeys(const std::string&, std::initializer_list<int>, bool = false);
		void setErrorMsg(const std::string&);
	private:
		void startInput(const char * = NULL);
		void endInput( );
		bool inputHandler(int);
		bool processInput(int);
		void resize( );
		bool isSuitable(int);
		std::vector<std::string> processCommand(const std::string&);
		void command(const std::string&, bool = true);
	private:
		std::map<int, std::pair<std::string, bool>> keymap;
		std::map<std::string, commandFn_t> cmdmap;
		std::stack<std::string> history;
		std::stack<std::string> undostory, redostory;
		char *_buf;
		int _i, _l, _idx;
		std::string errMsg;
};

#endif

