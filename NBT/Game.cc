#define GAME_MAIN
#include "Game.h"
#undef GAME_MAIN

using display::Terminal;

Game::Game(void) : _b(NULL), _x(0), _y(0)
{
}

Game::~Game(void)
{
	if(_b) delete _b;
}

void Game::init(void)
{
	CommandableScene::init();
}

void Game::input(int in)
{
	if(!_b) return;
	
	switch(in)
	{
		case display::Keys::LEFT:
			if(_x > 0) --_x;
			break;
		case display::Keys::RIGHT:
			if(_x < _b->width() - 1) ++_x;
			break;
		case display::Keys::UP:
			if(_y > 0) --_y;
			break;
		case display::Keys::DOWN:
			if(_y < _b->height() - 1) ++_y;
			break;
	}
}

void Game::update(int ms)
{
}

void Game::refresh(void)
{
	Terminal &terminal = Terminal::instance();

	if(_b)
	{
		std::wstring board = _b->draw(_x, _y);

		terminal.setCursorPos(0, 0);
		terminal.printfw(board.c_str());
		terminal.setCursorPos(_x + 1, _y + 1);
	}

	if(sideField.first.isRunning())
	{
		std::string d = sideField.first.getVisible();
		auto p = sideField.first.getPosition();
		terminal.setCursorPos(p.first, p.second);
		terminal.printf(d.c_str());
		p = sideField.first.getCurPos();
		terminal.setCursorPos(p.first, p.second);
	}

	CommandableScene::refresh();
}

void Game::finalize(void)
{
}

void Game::registerCommands(void)
{
	registerCommand("quit", std::bind(&Game::quit, this));
	registerCommand("new", std::bind(&Game::makeNew, this, std::placeholders::_1));
	registerCommand("solve", [this](std::vector<std::string> v) { _b->solve(); });
	registerCommand("clear", [this](std::vector<std::string> v) { if(_b) _b->clear(); _x = _y = 0; });
	registerCommand("delete", [this](std::vector<std::string> v) { delete _b; _b = NULL; _x = _y = 0; });
	registerCommand("seth", std::bind(&Game::editSide, this, true));
	registerCommand("setv", std::bind(&Game::editSide, this, false));
	registerCommand("save", [this](std::vector<std::string> v) { if(v.size() == 2 && _b) _b->save(v.at(1)); });
	registerCommand("load", [this](std::vector<std::string> v) { if(v.size() == 2) { if(!_b) _b = new Board(1, 1); _b->load(v.at(1)); } });

	assignKeys("quit", {'q'});
	assignKeys("solve", {'s', 'x'});
	assignKeys("seth", {'h'});
	assignKeys("setv", {'v'});
	assignKeys("clear", {'c'});
}

// # ---------------------------------------------------------------------------

void Game::makeNew(std::vector<std::string> p)
{
	if(p.size() != 3) return;

	int w, h;
	std::istringstream iss(p[1] + " " + p[2]);

	iss >> w;
	iss >> h;

	if(w < 0) w = -w;
	if(h < 0) h = -h;

	if(w && h) _b = new Board(w, h);
}

void Game::editSide(bool h)
{
	if(!_b) return;

	int x = 2 + _b->width() + 3, y = h ? (1 + _y) : (1 + _b->height() + _b->width() - _x);
	std::ostringstream oss;
	const int *side = _b->get(h ? 1 : 0, h ? _y : _x);

	for(int i = 0 ; side && side[i] ; ++i) oss << side[i] << ' ';

	_b->set(h ? 1 : 0, h ? _y : _x, NULL);

	std::string inV(oss.str());

	sideField.first.start(x, y, 9, inV.c_str());
	sideField.second = [this, h](const std::string& __v)->void
		{
			std::string _v(__v);
//			for(char &c : _v)
//			{
//				if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') c = ' ';
//			}
//			const char *__c = "`~!@#$%^&*()-_=+[{]}\\|\"';:/?.>,<";
//			while(*__c) std::replace(_v.begin(), _v.end(), *__c++, ' ');

			std::vector<int> v;
			std::istringstream iss(_v);

			while(iss)
			{
				int i = 0;
				iss >> i;
				v.push_back(i);
			}

			if(v.size() == 1)
			{
				_b->set(h ? 1 : 0, h ? _y : _x, NULL);

				return;
			}

			int *s = new int[v.size()];

			int *_s = s;
			for(int i : v)
			{
				*_s++ = i;
			}

			_b->set(h ? 1 : 0, h ? _y : _x, s);
		};

	pushInputFunction(std::bind(&Game::sideInput, this, std::placeholders::_1));
}

bool Game::sideInput(int in)
{
	if(_b && sideField.first.isRunning())
	{
		switch(in)
		{
			case display::Keys::TAB:
				if(_y < _b->height() - 1) ++_y; else _y = 0;
				if(_x < _b->width() - 1) ++_x; else _x = 0;
			case display::Keys::ENTER:
				sideField.second(sideField.first.getValue());
			case display::Keys::ESCAPE:
				sideField.first.clean();
				return true;
			default:
				sideField.first.input(in);
				break;
		}

		return false;
	}

	return true;
}

// # ===========================================================================

Board::Board(int w, int h) : _w(w), _h(h)
{
	_field = new int[w * h];
	_sides = new int**[2];
	_sides[0] = new int*[w];
	_sides[1] = new int*[h];

	memset(_field, 0, w * h * sizeof(*_field));
	memset(_sides[0], 0, w * sizeof(*_sides[0]));
	memset(_sides[1], 0, h * sizeof(*_sides[1]));
}

Board::~Board(void)
{
	destroy();
}

void Board::set(int s, int n, const int *v)
{
	if(_sides[s][n]) delete[] _sides[s][n];

	if(!v)
	{
		_sides[s][n] = NULL;

		return;
	}
	
	int l = 0;
	while(v[l++]);

	_sides[s][n] = new int[l];

	memcpy(_sides[s][n], v, l * sizeof(*v));
}

std::wstring Board::draw(int _x, int _y)
{
	std::wostringstream oss;

	display::BorderTypes<display::Borders::DOUBLE> border;
	display::BorderTypes<display::Borders::SINGLE> extra;

	oss << border.CORNER_TOPLEFT;

	for(int i = 0 ; i < _w ; ++i) oss << border.SIDES_HORIZONTAL;

	oss << border.CORNER_TOPRIGHT << '\n';

	for(int y = 0 ; y < _h ; ++y)
	{
		oss << border.SIDES_VERTICAL;

		for(int x = 0 ; x < _w ; ++x)
		{
			oss << (_field[x * _h + y] ? (_field[x * _h + y] == 1 ? '*' : 'X') : ' ');
		}

		oss << L'\u255f';//border.SIDES_VERTICAL 
		oss << extra.SIDES_HORIZONTAL;
		oss <<(y == _y ? L'>' : extra.SIDES_HORIZONTAL) << ' ';

		int i = 0;
		if(_sides[1][y])
			while(_sides[1][y][i])
				oss << _sides[1][y][i++] << ' ';

		oss << '\n';
	}

	oss << border.CORNER_BOTTOMLEFT;

	for(int i = 0 ; i < _w ; ++i) oss << L'\u2564';//border.SIDES_HORIZONTAL;

	oss << border.CORNER_BOTTOMRIGHT << "\n";

	for(int x = _w - 1 ; x >= 0 ; --x)
	{
		oss << ' ';
		for(int i = 0 ; i < x ; ++i) oss << extra.SIDES_VERTICAL;
		oss << extra.CORNER_BOTTOMLEFT << extra.SIDES_HORIZONTAL;

		for(int i = x ; i < _w ; ++i) oss << extra.SIDES_HORIZONTAL;

		oss << (x == _x ? L'>' : extra.SIDES_HORIZONTAL) << ' ';

		int i = 0;
		if(_sides[0][x])
			while(_sides[0][x][i])
				oss << _sides[0][x][i++] << ' ';

		oss << '\n';
	}

	return std::wstring(oss.str());
}

void Board::save(const std::string& fn)
{
	std::ofstream _out(fn, std::ios::out | std::ios::binary);
	auto out = [&_out](int c)
		{
			_out.write(reinterpret_cast<const char *>(&c), sizeof(int));
		};

	if(!_out.is_open()) return;

	out(2);

	for(int s = 0 ; s < 2 ; ++s)
	{
		out(s ? _h : _w);

		for(int n = 0 ; n < (s ? _h : _w) ; ++n)
		{
			if(_sides[s][n])
			{
				for(int i = 0 ; _sides[s][n][i] ; out(_sides[s][n][i++]));
			}

			out(0);
		}
	}

	_out.close();
}

void Board::load(const std::string& fn)
{
	std::ifstream _in(fn, std::ios::in | std::ios::binary);
	auto in = [&_in]()->int
		{
			int c;
			_in.read(reinterpret_cast<char *>(&c), sizeof(int));
			return c;
		};

	assert(_in.is_open());

	int __in_var = in();

	assert(__in_var==2);

	destroy();

	_sides = new int**[2];

	for(int s = 0 ; s < 2 ; ++s)
	{
		if(s) _h = in(); else _w = in();

		_sides[s] = new int*[s ? _h : _w];

		for(int n = 0 ; n < (s ? _h : _w) ; ++n)
		{
			std::vector<int> v;

			for(int i = 0 ; i = in() ; v.push_back(i)) if(_in.eof()) break;

			if(v.empty())
			{
				_sides[s][n] = NULL;
				continue;
			}

			v.push_back(0);

			int *t = new int[v.size()], *_t = t;

			for(int i : v)
			{
				*_t++ = i;
			}

			_sides[s][n] = t;
		}
	}

	_field = new int[_w * _h];
	clear();

	_in.close();
}

void Board::destroy(void)
{
	delete[] _field;

	if(!_sides) return;

	for(int i = 0 ; i < _w ; i++)
	{
		delete[] _sides[0][i];
	}
	for(int i = 0 ; i < _h ; i++)
	{
		delete[] _sides[1][i];
	}

	delete[] _sides[0];
	delete[] _sides[1];

	delete[] _sides;
}

Board::Board(const Board *b) : _w(b->_w), _h(b->_h)
{
	_field = new int[_w * _h];
	memcpy(_field, b->_field, _w * _h * sizeof(*_field));

	_sides = NULL;
}

bool Board::isSolved(void)
{
	for(int y = 0 ; y < _h ; ++y)
	{
		for(int x = 0 ; x < _w ; ++x)
		{
			if(!at(x, y)) return false;
		}
	}

	return _field;
}

void Board::clear(void)
{
	memset(_field, 0, _w * _h * sizeof(*_field));
}

void Board::solve(void)
{
	clear();

	for(int s = 0 ; s < 2 ; ++s)
	{
		for(int x = 0 ; x < (s ? _h : _w) ; ++x)
		{
			if(!_sides[s][x] || !*_sides[s][x])
			{
				for(int y = 0 ; y < (s ? _w : _h) ; ++y) at(s ? y : x, s ? x : y) = 2;
			}
			else if(_sides[s][x][0] == (s ? _w : _h))
			{
				for(int y = 0 ; y < (s ? _w : _h) ; ++y) at(s ? y : x, s ? x : y) = 1;
			}
			else
			{
				int l = 0, sum = 0;
				while(_sides[s][x][l]) sum += _sides[s][x][l++];
				sum += l - 1;

				if(sum == (s ? _w : _h))
				{
					int y = 0;
					for(int i = 0 ; i < l ; ++i)
					{
						for(int j = 0 ; j < _sides[s][x][i] ; ++j) at(x, y++) = 1;
						if(y < (s ? _w : _h)) at(x, y++) = 2;
					}
				}
			}
		}
	}

	const Board *that = this;

	if(!doSolve([this](const Board *b)->void
		{
			memcpy(_field, b->_field, _w * _h * sizeof(int));
		})) clear();
}

bool Board::checkValidity(void)
{
	
}

bool Board::doSolve(std::function<void(const Board *)> copySolution)
{
	auto tryBoard = [this, &copySolution](int x, int y, int v)->bool
		{
			Board b(this);

			b.at(x, y) = v;

			return b.doSolve(copySolution);
		};

	for(int y = 0 ; y < _h ; ++y)
	{
		for(int x = 0 ; x < _w ; ++x)
		{
			if(at(x, y))
			{
				if(checkValidity()) continue; else return false;
			}

			if(tryBoard(x, y, 1)) return true;
			if(tryBoard(x, y, 2)) return true;

			return false;
		}
	}

	copySolution(this);

	return true;
}

