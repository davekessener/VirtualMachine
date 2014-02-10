#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <sstream>
#include <cstring>
#include "Keyboard.h"
#include "Timer.h"
#include "Screen.h"

#define KEY_CTRL_C 3
#define KEY_CTRL_D 4
#define KEY_ESC 27

class Sudoku
{
	public:
		static void run(int, char **);
		void setPos(int, int);
		void draw( );
		void left( );
		void right( );
		void up( );
		void down( );
		void in(int);
		void advance( );
		void origin( );
		void setNumber(int, int, int);
		int countFilled( );
		int getPossibleSolutions(int, int, int *);
		void solve( );
		void clear( );
		void undo( );
		void write(const std::string&);
		void read(const std::string&);
		inline void fillAllObvious( ) { backup(); _solve(1); }
		inline void fillNext( ) { backup(); _solve(2); }
	private:
		Sudoku(Screen& s, Keyboard& kb);
		Sudoku(const Sudoku&);
		~Sudoku( );
		void init( );
		void redrawBoard( );
		std::string _solve(int = 0);
		void backup( );
		int x, y;
		int board[9][9];
		bool deleteErr, isOriginal;
		Screen &_s;
		Keyboard &_kb;
		std::string errStr;
		std::deque<int *> _backup;
};

#endif

