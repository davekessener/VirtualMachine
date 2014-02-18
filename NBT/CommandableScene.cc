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
	if(_buf)
	{
		display::Terminal &terminal = display::Terminal::instance();
		int h = terminal.getScreenHeight() - 1;

		terminal.setCursorPos(0, h);
		terminal.printf(":%s", _buf);
		terminal.setCursorPos(_idx + 1, h);
	}
}

bool CommandableScene::inputHandler(int in)
{
	if(in == ':')
	{
		startInput();
	}
	else if(keymap.count(in) > 0)
	{
		std::string cmd = keymap.at(in);
		if(cmdmap.count(cmd) > 0)
		{
			command(cmd);
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

void CommandableScene::assignKeys(const std::string& cmd, std::initializer_list<int> keys)
{
	for(int k : keys)
	{
		keymap[k] = cmd;
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

void CommandableScene::startInput(void)
{
	if(_buf) free(_buf);

	_buf = static_cast<char *>(malloc(_l = STD_BUFSIZE));
	_i = _idx = 0;

	memset(_buf, 0, _l);

	display::Terminal::instance().saveCursorPos();

	pushInputFunction(std::bind(&CommandableScene::processInput, this, std::placeholders::_1));
}

bool CommandableScene::processInput(int in)
{
	auto cleanup = [this]
		{
			free(_buf); _buf = NULL;
			_i = _idx = _l = 0;
			display::Terminal::instance().restoreCursorPos();
		};

	switch(in)
	{
		case display::Keys::LEFT:
			if(_idx > 0) --_idx;
			break;
		case display::Keys::RIGHT:
			if(_idx < _i) ++_idx;
			break;
		case display::Keys::UP:
			_idx = 0;
			break;
		case display::Keys::DOWN:
			_idx = _i;
			break;
		case display::Keys::BACKSPACE:
			if(_idx > 0)
			{
				int i = --_idx;
				while(i++ < _i) _buf[i - 1] = _buf[i];
				_buf[--_i] = '\0';
				break;
			}
		case display::Keys::ESCAPE:
			cleanup();
			return true;
		case display::Keys::ENTER:
			_buf[_i] = '\0';
			command(_buf);
			cleanup();
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

void CommandableScene::command(const std::string& cmd)
{
	std::vector<std::string> params = processCommand(cmd);

	if(params.empty()) return;

	if(cmdmap.count(params.at(0)) > 0)
	{
		cmdmap.at(params.at(0))(params);
	}
}

