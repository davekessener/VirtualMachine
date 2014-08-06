#ifndef VIEWER_H
#define VIEWER_H

#include <string>
#include "Screen.h"

class Viewer : public Screen
{
	using Screen::Controls;
	using Screen::modifier_t;
	using Screen::render_fn;

	public:
	enum class Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	public:
		Viewer( );
		virtual ~Viewer( ) noexcept;
		template<typename I>
			void load(I, I);
		void add(const std::string&);
		void finalize( );
		void clear( );
		void shift(Direction, bool = false);
		void shuffle( );
		void reset( );
		void next( );
		void back( );
		void toggleScale( );
		void scale(bool);
		void hide( );
	private:
		void loadImage(const std::string&);
		void i_setScreen(int, int);
		void i_suspend(bool);
		void i_keyPress(Controls, const modifier_t&);
	private:
		struct Impl;
		Impl *impl_;
};

template<typename I>
void Viewer::load(I i1, I i2)
{
	while(i1 != i2)
	{
		add(*i1);
		++i1;
	}
}

#endif

