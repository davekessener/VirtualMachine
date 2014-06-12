#include "Sudoku.h"

void Sudoku::run(int argc, char *argv[])
{
	Sudoku s(Screen::instance(), Keyboard::instance());
	Timer t;

	if(argc > 1)
	{
		s.read(argv[1]);
	}

	while(1)
	{
		s.draw();

		int c;
		while(Keyboard::instance().empty())
		{
			t.sleep(10);
		}
		c = Keyboard::instance().pop();
		Keyboard::instance().clear();

		switch(c)
		{
			case KEY_CTRL_C:
			case KEY_CTRL_D:
			case KEY_ESC:
			case 'q':
				goto end;
			case 'w': s.up();   break; 
			case 'a': s.left(); break; 
			case 's': s.down(); break; 
			case 'd': s.right(); break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				s.in(c - '0');
				break;
			case ' ':
				s.advance();
				break;
			case 'e':
				s.solve();
				break;
			case 'c':
				s.clear();
				break;
			case 'u':
				s.undo();
				break;
			case 'o':
				s.origin();
				break;
			case 'n':
				s.fillNext();
				break;
			case 'f':
				s.fillAllObvious();
				break;
			case 'y':
				s.write("sudoku.sk");
				break;
			case 'z':
				s.redrawBoard();
				break;
			default:
				;
		}
	}

end:
	;
}

Sudoku::Sudoku(Screen& s, Keyboard& kb) : _s(s), _kb(kb), x(0), y(0), errStr(""), deleteErr(false), isOriginal(true)
{
	static const wchar_t *field[19] = 
	{		
		(const wchar_t *) L"\u2554\u2550\u2564\u2550\u2564\u2550\u2566\u2550\u2564\u2550\u2564\u2550\u2566\u2550\u2564\u2550\u2564\u2550\u2557",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u255f\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u2562",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u255f\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u2562",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u2560\u2550\u256a\u2550\u256a\u2550\u256c\u2550\u256a\u2550\u256a\u2550\u256c\u2550\u256a\u2550\u256a\u2550\u2563",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u255f\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u2562",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u255f\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u2562",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u2560\u2550\u256a\u2550\u256a\u2550\u256c\u2550\u256a\u2550\u256a\u2550\u256c\u2550\u256a\u2550\u256a\u2550\u2563",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u255f\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u2562",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u255f\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u256b\u2500\u253c\u2500\u253c\u2500\u2562",
		(const wchar_t *) L"\u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551 \u2502 \u2502 \u2551",
		(const wchar_t *) L"\u255a\u2550\u2567\u2550\u2567\u2550\u2569\u2550\u2567\u2550\u2567\u2550\u2569\u2550\u2567\u2550\u2567\u2550\u255d"
	};

	for(int i = 0 ; i < 9 ; ++i)
	{
		for(int j = 0 ; j < 9 ; ++j)
		{
			board[i][j] = 0;
		}
	}

	_s.clear();

	for(int i = 0 ; i < 19 ; i++)
	{
		_s.setPos(0, i);
		_s.puts(field[i]);
	}

	setPos(0, 0);

	_s.flush();
}

Sudoku::Sudoku(const Sudoku& s) : _s(s._s), _kb(s._kb), x(0), y(0), errStr(""), deleteErr(false), isOriginal(false)
{
	std::memcpy(board, s.board, 9 * 9 * sizeof(int));
}

Sudoku::~Sudoku(void)
{
	while(!_backup.empty())
	{
		delete[] _backup[0];
		_backup.pop_front();
	}
}

void Sudoku::draw(void)
{
	if(!isOriginal) return;

	if(deleteErr || !errStr.empty())
	{
		_s.setPos(0, _s.height() - 1);
		_s.eraseLine();

		deleteErr = false;

		if(!errStr.empty())
		{
			_s << errStr;
			errStr.clear();
			deleteErr = true;
		}

		setPos(x, y);
	}

	_s.flush();
}

int Sudoku::countFilled(void)
{
	int c = 0;

	for(int i = 0 ; i < 9 ; ++i)
	{
		for(int j = 0 ; j < 9 ; ++j)
		{
			if(board[i][j]) ++c;
		}
	}

	return c;
}

void Sudoku::solve(void)
{
	if(countFilled() < 17)
	{
		errStr = std::string("Err: Braucht mindestens 17 vorgegebene Zahlen.");

		return;
	}

	origin();
	backup();

	errStr = _solve();
}

std::string Sudoku::_solve(int solveBehavior)
{
	int _x, _y, _sc;

redo:
	_x = _y = -1;
	_sc = 10;

	if(countFilled() == 81)
	{
		return std::string("");
	}

	for(int j = 0 ; j < 9 ; ++j)
	{
		for(int i = 0 ; i < 9 ; ++i)
		{
			if(board[i][j]) continue;

			int sol[9];
			int sc = getPossibleSolutions(i, j, sol);

			if(sc == 0)
			{
				char buf[64];
				std::sprintf(buf, "ERR: Feld @(%d|%d) hat keine Loesung.", i, j);

				return std::string(buf);
			}
			else if(sc == 1)
			{
				setNumber(i, j, sol[0]);

				goto redo;
			}
			else if(sc < _sc)
			{
				_sc = sc;
				_x = i;
				_y = j;
			}
		}
	}

	if(solveBehavior == 1) return std::string("");

	if(_x < 0 || _y < 0) return std::string("ERR: Keine Loesung moeglich.");

	int sol[9];
	if(_sc != getPossibleSolutions(_x, _y, sol)) return std::string("ERR: Inconsistent field. Abort.");

	for(int i = 0 ; i < _sc ; ++i)
	{
		Sudoku clone(*this);
		clone.board[_x][_y] = sol[i];
		clone._solve();

		if(clone.countFilled() == 81)
		{
			if(solveBehavior == 2)
			{
				setNumber(_x, _y, sol[i]);
			}
			else
			{
				std::memcpy(board, clone.board, 9 * 9 * sizeof(int));
				redrawBoard();
			}

			return std::string("");
		}
	}

	return std::string("ERR: Unknown error.");
}

int Sudoku::getPossibleSolutions(int _x, int _y, int *_sol)
{
	std::vector<int> sol = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto erase = [&sol](int v)->void
		{
			for(auto i = sol.begin() ; i != sol.end() ; ++i)
			{
				if(*i == v)
				{
					sol.erase(i);
					break;
				}
			}
		};

	for(int x = 0 ; x < 9 ; ++x)
	{
		if(board[x][_y]) erase(board[x][_y]);
	}

	for(int y = 0 ; y < 9 ; ++y)
	{
		if(board[_x][y]) erase(board[_x][y]);
	}

	int sx = _x / 3, sy = _y / 3;

	for(int y = sy * 3 ; y < (sy + 1) * 3 ; ++y)
	{
		for(int x = sx * 3 ; x < (sx + 1) * 3 ; ++x)
		{
			if(board[x][y]) erase(board[x][y]);
		}
	}

	for(int i = 0 ; i < sol.size() ; ++i)
	{
		_sol[i] = sol[i];
	}

	return sol.size();
}

void Sudoku::clear(void)
{
	origin();
	if(countFilled() == 0) return;

	backup();
	std::memset(board, 0, 9 * 9 * sizeof(int));
	redrawBoard();
}

void Sudoku::undo(void)
{
	if(_backup.empty())
	{
		errStr = std::string("ERR: Kein Backup verfuegbar.");
		return;
	}
	
	int *buf = _backup[_backup.size() - 1];
	_backup.pop_back();

	for(int i = 0 ; i < 9 ; ++i)
	{
		std::memcpy(board[i], buf + i * 9, 9 * sizeof(int));
	}

	redrawBoard();

	delete[] buf;
}

void Sudoku::backup(void)
{
	int *buf = new int[9 * 9];

	for(int i = 0 ; i < 9 ; ++i)
	{
		std::memcpy(buf + i * 9, board[i], 9 * sizeof(int));
	}

	_backup.push_back(buf);
}

void Sudoku::redrawBoard(void)
{
	if(!isOriginal) return;

	for(int y = 0 ; y < 9 ; ++y)
	{
		for(int x = 0 ; x < 9 ; ++x)
		{
			setPos(x, y);
			_s << (char) (board[x][y] ? ('0' + board[x][y]) : ' ');
		}
	}

	origin();
	setPos(0, 0);
}

void Sudoku::setPos(int _x, int _y)
{
	if(isOriginal)
	{
		_s.setPos(1 + _x * 2, 1 + _y * 2);
	}
}

void Sudoku::left(void)
{
	if(x > 0)
	{
		setPos(--x, y);
	}
}

void Sudoku::right(void)
{
	if(x < 8)
	{
		setPos(++x, y);
	}
}

void Sudoku::up(void)
{
	if(y > 0)
	{
		setPos(x, --y);
	}
}

void Sudoku::down(void)
{
	if(y < 8)
	{
		setPos(x, ++y);
	}
}

void Sudoku::in(int n)
{
	if(!isOriginal) return;

	n %= 10;

	if(board[x][y] != n)
	{
		_s << (char)((board[x][y] = n) ? board[x][y] + '0' : ' ');
		setPos(x, y);
	}
}

void Sudoku::advance(void)
{
	if(++x == 9)
	{
		x = 0;
		if(y < 8) ++y; else x = 8;
	}

	setPos(x, y);
}

void Sudoku::origin(void)
{
	if(x && y)
	{
		setPos(x = 0, y = 0);
	}
}

void Sudoku::setNumber(int _x, int _y, int v)
{
	if(board[_x][_y] != v)
	{
		setPos(_x, _y);

		board[_x][_y] = v;

		if(isOriginal)
		{
			_s << v;
			setPos(x, y);
		}
	}
}

void Sudoku::write(const std::string& fn)
{
	std::ofstream out(fn, std::ios::out | std::ios::binary);

	if(!out.is_open())
	{
		errStr = std::string("ERR: Couldn't open file '") + fn + "'!";
		return;
	}

	for(int _y = 0 ; _y < 9 ; ++_y)
	{
		for(int _x = 0 ; _x < 9 ; ++_x)
		{
			if(!board[_x][_y]) continue;
			out.put(_x + 9 * _y);
			out.put(board[_x][_y]);
		}
	}

	out.put(0xff);

	out.close();

	errStr = std::string("Successfully wrote file '") + fn + "'";
}

void Sudoku::read(const std::string& fn)
{
	std::ifstream in(fn, std::ios::in | std::ios::binary);

	if(!in.is_open())
	{
		errStr = std::string("ERR: Couldn't open file '") + fn + "'!";
		return;
	}

	int c;
	while((c = in.get()) != 0xff && !in.eof())
	{
		int _x = c % 9, _y = c / 9;
		c = in.get();

		if(c > 9)
		{
			std::stringstream ss;
			ss << "ERR: Read invalid value '" << c << "'!";
			errStr = ss.str();
			in.close();
			return;
		}

		board[_x][_y] = c;
	}

	in.close();

	errStr = std::string("Successfully read file'") + fn + "'";

	redrawBoard();
}

int main(int argc, char *argv[])
{
	Sudoku::run(argc, argv);

	return 0;
}

