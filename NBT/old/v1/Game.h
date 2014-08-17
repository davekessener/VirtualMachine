#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include "CommandableScene.h"
#include "InputField.h"

#ifdef GAME_MAIN
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <cassert>
#include <cstring>
#include "Display.h"
#include "Terminal.h"

class Board
{
	public:
		Board(int, int);
		~Board( );
		const int *get(int s, int n) { return _sides[s][n]; }
		void set(int, int, const int *);
		int width( ) { return _w; }
		int height( ) { return _h; }
		const int *operator[](int x) { return _field + _h * x; }
		std::wstring draw(int, int);
		void save(const std::string&);
		void load(const std::string&);
		bool isSolved( );
		void clear( );
		void solve( );
	private:
		Board(const Board *);
		inline int& at(int x, int y) { return _field[y + x * _h]; }
		void destroy( );
		bool checkValidity( );
		bool doSolve(std::function<void(const Board *)>);
		int _w, _h;
		int *_field;
		int ***_sides;
};
#else
class Board;
#endif

class Game : public CommandableScene
{
	public:
		Game( );
		~Game( );
		void init( );
		void input(int);
		void update(int);
		void refresh( );
		void finalize( );
		void registerCommands( );
	private:
		void makeNew(std::vector<std::string>);
		void editSide(bool);
		bool sideInput(int);
		Board *_b;
		int _x, _y;
		std::pair<InputField, std::function<void(const std::string&)>> sideField;
};

#endif

