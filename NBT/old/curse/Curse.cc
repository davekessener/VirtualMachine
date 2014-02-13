#define CURSE_MAIN
#include "Curse.h"
#undef CURSE_MAIN

namespace ncurses
{
	Curse::_curse_config::_curse_config(void) : bmode(CharBufferMode::COOKED), _timeout(-1)
	{ 
	}

	Curse::_curse_config::_curse_config(CharBufferMode bm, AttrMap& a, int _t) : bmode(bm), attr(a), _timeout(_t)
	{ 
	}

	Curse::_curse_config::_curse_config(const Curse::_curse_config& cc) : attr(cc.attr), bmode(cc.bmode), _timeout(cc._timeout)
	{ 
	}

	Curse::_curse_config::~_curse_config(void)
	{ 
	}

	Curse::_curse_config& Curse::_curse_config::operator=(const Curse::_curse_config& cc)
	{ 
		bmode = cc.bmode;
		_timeout = cc._timeout;

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

	int Curse::_curse_config::getTimeout(void) const
	{ 
		return _timeout;
	}

	void Curse::_curse_config::setAttr(BufferAttribute ba, bool f)
	{ 
		attr[ba] = f;
	}

	void Curse::_curse_config::setBufferMode(CharBufferMode bm)
	{ 
		bmode = bm;
	}

	void Curse::_curse_config::setTimeout(int t)
	{ 
		_timeout = t;
	}

// # ---------------------------------------------------------------------------

	void Curse::begin(void)
	{ 
#ifdef DEBUG
		assert(!_is_running);
#endif

		if(!_is_running)
		{
			setlocale(LC_ALL, "");
			initscr();
			keypad(stdscr, true);
			_is_running = true;
		}
	}
	
	void Curse::end(void)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif
		
		if(_is_running)
		{
			endwin();
			_is_running = false;
		}
	}
	
	void Curse::setBufferMode(CharBufferMode mode)
	{
#ifdef DEBUG
		assert(_is_running);
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
		assert(_is_running);
#endif

		std::function<void(void)> enable, disable;

		switch(ba)
		{
			case BufferAttribute::ECHO:
				enable = &echo;
				disable = &noecho;
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
		assert(_is_running);
#endif

		int w, h;

		getmaxyx(stdscr, h, w);

		return Curse::Size(w, h);
	}

	Curse::Position Curse::getCursorPosition(void) const
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		int x, y;

		getyx(stdscr, y, x);

		return Curse::Position(x, y);
	}

	void Curse::moveCursor(int x, int y)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		move(y, x);
	}

	void Curse::eraseToEndOfLine(void)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		clrtoeol();
	}

	void Curse::putch(int ch)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		addch(ch);
	}

	void Curse::print(const char *s)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		addstr(s);
	}

	void Curse::printw(const wchar_t *s)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		addwstr(s);
	}

	void Curse::printf(const char *s, ...)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		va_list l;
		va_start(l, s);

		vwprintw(stdscr, s, l);

		va_end(l);
	}

	void Curse::printfw(const wchar_t *s, ...)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		va_list l;
		va_start(l, s);

		vprintfw(s, l);

		va_end(l);
	}

	void Curse::vprintf(const char *s, va_list l)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		vwprintw(stdscr, s, l);
	}

	void Curse::vprintfw(const wchar_t *s, va_list l)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		wchar_t buf[1024];
		vswprintf(buf, sizeof(buf) / sizeof(wchar_t), s, l);

		addwstr(buf);
	}

	wchar_t Curse::getch(void)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif
		wint_t c;

		get_wch(&c);
		
		return static_cast<wchar_t>(c);
	}

	int Curse::getline(char *s, int n)
	{ 
#ifdef DEBUG
		assert(_is_running);
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
		assert(_is_running);
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

	CharBufferMode Curse::getBufferMode(void) const
	{ 
		return buffermode;
	}

	bool Curse::isBufferAttributeSet(BufferAttribute ba) const
	{ 
		return attributes.count(ba) > 0 ? attributes.at(ba) : true;
	}

	Curse::Configuration Curse::backupConfiguration(void)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		config = Configuration(buffermode, attributes, _timeout);

		return config;
	}

	void Curse::restoreConfiguration(void)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		restoreConfiguration(config);
	}

	void Curse::restoreConfiguration(const Curse::Configuration& c)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		setBufferMode(c.bmode);
		setTimeout(c._timeout);

		for(Configuration::Attr a : c.attr)
		{
			setBufferAttribute(a.first, a.second ? SetMode::SET : SetMode::RESET);
		}
	}

	void Curse::flush(void)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		refresh();
	}

	void Curse::pause(void)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		flush();

		CharBufferMode bm = buffermode;
		if(_timeout >= 0) timeout(-1);
		if(buffermode != CharBufferMode::COOKED) setBufferMode(CharBufferMode::COOKED);

		getch();

		if(bm != buffermode) setBufferMode(bm);
		if(_timeout >= 0) setTimeout(_timeout);

		fflush(stdin);
	}

	void Curse::setTimeout(int to)
	{ 
#ifdef DEBUG
		assert(_is_running);
#endif

		timeout(_timeout = to);
	}
	
	Curse& Curse::instance(void)
	{
		static Curse c;

		static_assert(sizeof(wint_t)==sizeof(wchar_t), "ERR: 'wchar_t' and 'wint_t' are not compadible.");

		return c;
	}
	
	Curse::Curse(void) : config(), _is_running(false), _timeout(-1)
	{
		attributes[BufferAttribute::ECHO] = true;
		attributes[BufferAttribute::SHOW_CURSOR] = true;
	}
	
	Curse::~Curse(void)
	{
		if(_is_running)
		{
			end();
		}
	}
}

