#define CMDSCENE_MAIN
#include "CommandableScene.h"
#undef CMDSCENE_MAIN

CommandableScene::CommandableScene(void) : _buf(NULL), _i(0), _l(0), _idx(0)
{
	pushInputFunction(std::bind(&CommandableScene::inputHandler, this, std::placeholders::_1));
}

CommandableScene::~CommandableScene(void)
{
	if(_buf) free(_buf); _buf = NULL;
}

void CommandableScene::init(void)
{
	registerCommands();
}

void CommandableScene::refresh(void)
{
	display::Terminal &terminal = display::Terminal::instance();
	int h = terminal.getScreenHeight() - 1;

	if(!errMsg.empty())
	{
		terminal.saveCursorPos();
		terminal.setCursorPos(0, h);
		terminal.printf("ERR: %s", errMsg.c_str());
		terminal.restoreCursorPos();
	}
	else if(_buf)
	{
		terminal.setCursorPos(0, h);
		terminal.printf(":%s", _buf);
		terminal.setCursorPos(_idx + 1, h);
	}
}

bool CommandableScene::inputHandler(int in)
{
	errMsg.clear();

	if(in == ':')
	{
		startInput();
	}
	else if(keymap.count(in) > 0)
	{
		std::string cmd = keymap.at(in).first;
		if(cmdmap.count(cmd) > 0)
		{
			if(keymap.at(in).second)
			{
				startInput((cmd + " ").c_str());
			}
			else
			{
				command(cmd, false);
			}
		}
	}
	else
	{
		input(in);
	}

	return false;
}

void CommandableScene::registerCommand(const std::string& cmd, commandFn_t f)
{
	cmdmap[cmd] = f;
}

void CommandableScene::assignKeys(const std::string& cmd, std::initializer_list<int> keys, bool keepOpen)
{
	if(cmd.empty()) return;

	for(int k : keys)
	{
		keymap[k] = std::make_pair(cmd, keepOpen);
	}
}

bool CommandableScene::isSuitable(int ch)
{
	if(ch >= 'a' && ch <= 'z' ||
		ch >= 'A' && ch <= 'Z' ||
		ch >= '0' && ch <= '9')
		return true;

	const char *s = " `~!@#$%^&*()_+-={}|[]\\;:'\"/?.>,<";
	while(*s) if(*s++ == ch) return true;

	return false;
}

void CommandableScene::setErrorMsg(const std::string& err)
{
	errMsg = err;
	Logger::log("[CMD] ERR: %s", err.c_str());
}

// # ---------------------------------------------------------------------------

void CommandableScene::startInput(const char *_init)
{
	if(_buf) free(_buf);

	if(!_init)
	{
		_buf = static_cast<char *>(malloc(_l = STD_BUFSIZE));
		_i = _idx = 0;
	
		memset(_buf, 0, _l);
	}
	else
	{
		_buf = strdup(_init);
		_i = _idx = strlen(_buf);
		_l = _i + 1;
	}

	undostory = history;

	display::Terminal::instance().saveCursorPos();

	pushInputFunction(std::bind(&CommandableScene::processInput, this, std::placeholders::_1));
}

void CommandableScene::endInput(void)
{
	free(_buf); _buf = NULL;
	_i = _idx = _l = 0;
	while(!undostory.empty()) undostory.pop();
	while(!redostory.empty()) redostory.pop();
	display::Terminal::instance().restoreCursorPos();
}

bool CommandableScene::processInput(int in)
{
	switch(in)
	{
		case display::Keys::LEFT:
			if(_idx > 0) --_idx;
			break;
		case display::Keys::RIGHT:
			if(_idx < _i) ++_idx;
			break;
		case display::Keys::UP:
			if(undostory.empty()) break;
			redostory.push(std::string(_buf));
			free(_buf);
			_buf = strdup(undostory.top().c_str());
			_i = undostory.top().length();
			_l = _i + 1;
			if(_l == 1) resize();
			if(_idx > _i) _idx = _i;
			undostory.pop();
			break;
		case display::Keys::DOWN:
			if(redostory.empty()) break;
			undostory.push(std::string(_buf));
			free(_buf);
			_buf = strdup(redostory.top().c_str());
			_i = redostory.top().length();
			_l = _i + 1;
			if(_l == 1) resize();
			if(_idx > _i) _idx = _i;
			redostory.pop();
			break;
		case display::Keys::BACKSPACE:
			if(_idx > 0)
			{
				int i = --_idx;
				while(i++ < _i) _buf[i - 1] = _buf[i];
				_buf[--_i] = '\0';
				break;
			}
			else if(_i > 0) break;
		case display::Keys::ESCAPE:
			endInput();
			return true;
		case display::Keys::ENTER:
			_buf[_i] = '\0';
			command(_buf);
			endInput();
			return true;
		default:
			if(isSuitable(in))
			{
				int i = _i;
				while(i-- > _idx) _buf[i + 1] = _buf[i];
				_buf[_idx++] = in;
				if(++_i == _l - 1) resize();
			}
			break;
	}

	return false;
}

void CommandableScene::resize(void)
{
	_l <<= 1;
	_buf = static_cast<char *>(realloc(_buf, _l));
	memset(_buf + _i, 0, _l - _i);
}

std::vector<std::string> CommandableScene::processCommand(const std::string& cmd)
{
	std::vector<std::string> v;
	std::stringstream iss(cmd);

	while(iss)
	{
		std::string s;
		iss >> s;
		if(!s.empty()) v.push_back(s);
	}

	return v;
}

void CommandableScene::command(const std::string& cmd, bool doHistory)
{
	std::vector<std::string> params = processCommand(cmd);

	if(params.empty()) return;

	if(doHistory) history.push(cmd);

	if(cmdmap.count(params.at(0)) > 0)
	{
		cmdmap.at(params.at(0))(params);
	}
	else
	{
		setErrorMsg(std::string("Command '") + params.at(0) + "' isn't recognized.");
	}
}

