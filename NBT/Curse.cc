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

	void Curse::play(Curse *curse)
	{
		static unsigned int rcount = 0;

		if(++rcount == 1)
		{
			setlocale(LC_ALL, "");
			curse->start();
			curse->showCursor(false);
		}

		curse->run();

		if(--rcount == 0)
		{
			curse->end();
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

	void Curse::drawBorder(int x1, int y1, int x2, int y2, wchar_t c_u, wchar_t c_r, wchar_t c_ul, wchar_t c_ur, wchar_t c_dl, wchar_t c_dr)
	{
		int w = x2 - x1 + 1, h = y2 - y1 + 1;

		wchar_t *b = static_cast<wchar_t *>(malloc((w + 1) * sizeof(wchar_t)));
		for(wchar_t *_t = b + w ; _t != b ; *--_t = c_u);
		b[w] = L'\0';

		wchar_t *s = static_cast<wchar_t *>(malloc((w + 1) * sizeof(wchar_t)));
		for(wchar_t *_t = s + w ; _t != s ; *--_t = L' ');
		s[w] = L'\0';

		for(int y = y1 ; y <= y2 ; ++y)
		{
			wchar_t *_s = (y == y1 || y == y2) ? b : s;
			_s[0] = (y == y1 ? c_ul : (y == y2 ? c_dl : c_r));
			_s[w - 1] = (y == y1 ? c_ur : (y == y2 ? c_dr : c_r));
			setCursorPos(x1, y);
			wprintf(_s);
		}

		free(s);
		free(b);
	}
}

