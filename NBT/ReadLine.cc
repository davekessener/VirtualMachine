#define CURSE_READLINE_MAIN
#include "ReadLine.h"
#undef CURSE_READLINE_MAIN

namespace ncurses
{
	ReadLine::ReadLine(Curse& _curse, onDoneFn _onDone, cleanupFn _cleanup, onFailureFn _onFailure) :
		curse(_curse), onDone(_onDone), cleanup(_cleanup), onFailure(_onFailure)
	{
        buf = NULL;
		l = i = idx = 0;
        curse.getCursorPos(x, y);
	}

	ReadLine::~ReadLine(void)
	{
		free(buf);
	}

	bool ReadLine::isSuitable(int ch)
	{
		if(ch >= 'a' && ch <= 'z' ||
			ch >= 'A' && ch <= 'Z' ||
			ch >= '0' && ch <= '9')
            return true;

        const char *s = " `~!@#$%^&*()_+-={}|[]\\;:'\"/?.>,<";
        while(*s) if(*s++ == ch) return true;

        return false;
	}

	bool ReadLine::operator()(int ch)
	{
        if(l == 0) resize();

		switch(ch)
		{
			case Keys::LEFT:
				if(idx > 0) --idx;
				break;
			case Keys::RIGHT:
				if(idx < i) ++idx;
				break;
			case Keys::UP:
				idx = 0;
				break;
			case Keys::DOWN:
				idx = i;
				break;
			case Keys::BACKSPACE:
				if(idx > 0)
				{
					int _i = --idx;
					while(_i++ < i) buf[_i - 1] = buf[_i];
					buf[--i] = ' ';
				}
				break;
			case Keys::ESCAPE:
				memset(buf, ' ', i);
				i = -1;
				break;
            case '\n':
                buf[i] = '\0';
                onDone(buf);
				if(static_cast<bool>(cleanup)) cleanup();
				return true;
			default:
                if(isSuitable(ch))
                {
                    int _i = i;
                    while(_i-- > idx) buf[_i + 1] = buf[_i];
                    buf[idx++] = ch;
                    if(++i == l) resize();
                }
                break;
		}

        curse.setCursorPos(x, y);
		curse.printf(buf);
		curse.setCursorPos(x + idx, y);

		if(i < 0)
		{
			if(static_cast<bool>(onFailure)) onFailure();
			if(static_cast<bool>(cleanup)) cleanup();
			return true;
		}

        return false;
	}

	void ReadLine::resize(void)
	{
        if(l == 0) l = BUF_SIZE >> 1;

		l <<= 1;
		buf = reinterpret_cast<char *>(realloc(buf, l));
		memset(buf + i, 0, l - i);
	}
}

