#ifndef VIEWER_H
#define VIEWER_H

#include <functional>
#include <string>

class Viewer
{
	typedef std::function<void(void)> render_fn;

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
		~Viewer( );
		void setScreen(int, int);
		template<typename I>
			void load(I, I);
		void add(const std::string&);
		void finalize( );
		void clear( );
		bool ready( ) const { return static_cast<bool>(render_); };
		void suspend(bool = true);
		void shift(Direction, bool = false);
		void render( ) const { render_(); }
		void shuffle( );
		void reset( );
		void next( );
		void back( );
		void toggleScale( );
		void scale(bool);
		void hide( );
	private:
		void loadImage(const std::string&);
	private:
		render_fn render_;
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

