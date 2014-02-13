#define CURSE_MAIN
#include "Curse.h"
#undef CURSE_MAIN

namespace ncurses
{
	Curse::Curse(void) : running(false), timer()
	{
		setDefaultInput();
	}

	Curse::~Curse(void)
	{
	}

	void Curse::play(Curse& curse)
	{
		static unsigned int running = 0;

		if(++running == 1)
		{
			setlocale(LC_ALL, "");
			curse.start();
			curse.showCursor(false);
		}

		curse.run();

		if(--running == 0)
		{
			curse.end();
		}
	}

	void Curse::run()
	{
		running = true;

		init();

		timer.reset();
		while(running)
		{
			int ch, delta = timer.elapsed();
			timer.reset();

			if((ch = getChar()) != CurseBase::ERR)
			{
				if(_input(ch)) setDefaultInput();
			}

			update(delta);

			refresh();

			CurseBase::refresh();

			delta = 16 - (delta + timer.elapsed());
			if(delta > 0) timer.sleep(delta);
		}

		finalize();
	}

	void Curse::init(void)
	{
		setEcho(false);
		setCBreak(true);
		setTimeout(0);
	}

	void Curse::finalize(void)
	{
	}

	void Curse::quit(void)
	{
		running = false;
	}

	void Curse::setInputFunction(inputFn in)
	{
		_input = in;
	}

	void Curse::setDefaultInput(void)
	{
		_input = [this](int ch)->bool
			{
				this->input(ch);
				return false;
			};
	}
}

