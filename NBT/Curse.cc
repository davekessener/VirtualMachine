#include <assert.h>
#include <functional>
#include <stdarg.h>
#include <cwchar>
#include <string.h>
#include "Curse.h"

#include <ncurses.h>

#undef getch
#undef ncurses

namespace ncurses
{
	Curse::_curse_config::_curse_config(void) : bmode(CharBufferMode::COOKED)
	{
	}

	Curse::_curse_config::_curse_config(CharBufferMode bm, AttrMap& a) : bmode(bm), attr(a)
	{
	}

	Curse::_curse_config::_curse_config(const Curse::_curse_config& cc) : attr(cc.attr), bmode(cc.bmode)
	{
	}

	Curse::_curse_config::~_curse_config(void)
	{
	}

	Curse::_curse_config& Curse::_curse_config::operator=(const Curse::_curse_config& cc)
	{
		bmode = cc.bmode;

		attr.clear();
		for(Attr a : cc.attr)
		{
			attr[a.first] = a.second;
		}

		return *this;
	}

	bool Curse::_curse_config::getAttr(BufferAttribute ba) const
	{
		return attr.count(ba) > 0 ? attr.at(ba) : true;
	}

	CharBufferMode Curse::_curse_config::getBufferMode(void) const
	{
		return bmode;
	}

	void Curse::_curse_config::setAttr(BufferAttribute ba, bool f)
	{
		attr[ba] = f;
	}

	void Curse::_curse_config::setBufferMode(CharBufferMode bm)
	{
		bmode = bm;
	}

// # ---------------------------------------------------------------------------

	void Curse::begin(void)
	{
#ifdef DEBUG
		assert(!isRunning);
#endif

		if(!isRunning)
		{
			initscr();
			isRunning = true;
		}
	}
	
	void Curse::end(void)
	{
#ifdef DEBUG
		assert(isRunning);
#endif
		
		if(isRunning)
		{
			endwin();
			isRunning = false;
		}
	}
	
	void Curse::setBufferMode(CharBufferMode mode)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		if(buffermode == mode) return;

		switch(buffermode)
		{
			case CharBufferMode::CBREAK: nocbreak(); break;
			case CharBufferMode::RAW: noraw(); break;
		}

		buffermode = mode;

		switch(buffermode)
		{
			case CharBufferMode::CBREAK: cbreak(); break;
			case CharBufferMode::RAW: raw(); break;
		}
	}

	void Curse::setBufferAttribute(BufferAttribute ba, SetMode sm)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		std::function<void(void)> enable, disable;

		switch(ba)
		{
			case BufferAttribute::ECHO:
				enable = &echo;
				disable = &noecho;
				break;
			case BufferAttribute::DELAY:
				enable = []()->void { notimeout(stdscr, true); };
				disable = []()->void { notimeout(stdscr, false); };
				break;
			case BufferAttribute::SHOW_CURSOR:
				enable = []()->void { curs_set(1); };
				disable = []()->void { curs_set(0); };
				break;
#ifdef DEBUG
			default: assert(false);
#endif
		}

		if(sm == SetMode::SET)
		{
			enable();
			attributes[ba] = true;
		}
		else if(sm == SetMode::RESET)
		{
			disable();
			attributes[ba] = false;
		}
		else if(sm == SetMode::TOGGLE)
		{
			bool set = isBufferAttributeSet(ba);

			if(set)
			{
				disable();
			}
			else
			{
				enable();
			}
			
			attributes[ba] = !set;;
		}
#ifdef DEBUG
		else assert(false);
#endif
	}

	Curse::Size Curse::getScreenSize(void) const
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		int w, h;

		getmaxyx(stdscr, h, w);

		return Curse::Size(w, h);
	}

	Curse::Position Curse::getCursorPosition(void) const
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		int x, y;

		getyx(stdscr, y, x);

		return Curse::Position(x, y);
	}

	void Curse::moveCursor(int x, int y)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		move(y, x);
	}

	void Curse::putch(int ch)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		addch(ch);
	}

	void Curse::print(const char *s)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		addstr(s);
	}

	void Curse::printw(const wchar_t *s)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		addwstr(s);
	}

	void Curse::printf(const char *s, ...)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		va_list l;
		va_start(l, s);

		vwprintw(stdscr, s, l);

		va_end(l);
	}

	void Curse::printfw(const wchar_t *s, ...)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		va_list l;
		va_start(l, s);

		wchar_t buf[1024];
		vswprintf(buf, sizeof(buf) / sizeof(wchar_t), s, l);

		va_end(l);

		addwstr(buf);
	}

	wchar_t Curse::getch(void)
	{
#ifdef DEBUG
		assert(isRunning);
#endif
		wint_t c;

		get_wch(&c);
		
		return static_cast<wchar_t>(c);
	}

	int Curse::getline(char *s, int n)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		if(n > 0)
		{
			getnstr(s, n);
		}
		else
		{
			getstr(s);
		}

		return strlen(s);
	}

	int Curse::getlinew(wchar_t *s, int n)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		if(n > 0)
		{
			getn_wstr(reinterpret_cast<wint_t *>(s), n);
		}
		else
		{
			get_wstr(reinterpret_cast<wint_t *>(s));
		}

		return wcslen(s);
	}

	CharBufferMode Curse::getBufferMode(void)
	{
		return buffermode;
	}

	bool Curse::isBufferAttributeSet(BufferAttribute ba)
	{
		return attributes.count(ba) > 0 ? attributes[ba] : true;
	}

	Curse::Configuration Curse::backupConfiguration(void)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		config = Configuration(buffermode, attributes);

		return config;
	}

	void Curse::restoreConfiguration(void)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		restoreConfiguration(config);
	}

	void Curse::restoreConfiguration(const Curse::Configuration& c)
	{
#ifdef DEBUG
		assert(isRunning);
#endif

		setBufferMode(c.bmode);

		for(Configuration::Attr a : c.attr)
		{
			setBufferAttribute(a.first, a.second ? SetMode::SET : SetMode::RESET);
		}
	}
	
	Curse& Curse::instance(void)
	{
		static Curse c;

		static_assert(sizeof(wint_t)==sizeof(wchar_t), "ERR: 'wchar_t' and 'wint_t' are not compadible.");

		return c;
	}
	
	Curse::Curse(void) : config(), isRunning(false)
	{
		attributes[BufferAttribute::ECHO] = true;
		attributes[BufferAttribute::DELAY] = true;
		attributes[BufferAttribute::SHOW_CURSOR] = true;
	}
	
	Curse::~Curse(void)
	{
		if(isRunning)
		{
			end();
		}
	}
}

