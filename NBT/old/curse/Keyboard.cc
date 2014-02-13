#define CURSE_KEYBOARD_MAIN
#include "Keyboard.h"
#undef CURSE_KEYBOARD_MAIN

#define BUF_SIZE 1024

namespace ncurses
{
	void Keyboard::pause(void)
	{
		if(curse.isRunning())
		{
			curse.pause();
		}
		else
		{
			getc(stdin);
			fflush(stdin);
		}
	}

	std::string Keyboard::getline(void)
	{
		char buf[BUF_SIZE];

		curse.backupConfiguration();
		curse.setTimeout(-1);
		curse.setBufferMode(CharBufferMode::COOKED);
		curse.setBufferAttribute(BufferAttribute::SHOW_CURSOR);
		curse.setBufferAttribute(BufferAttribute::ECHO);

		curse.flush();

		curse.getline(buf, BUF_SIZE);

		buf[BUF_SIZE - 1] = '\0';

		curse.restoreConfiguration();

		return std::string(buf);
	}

	std::string Keyboard::readCommand(void)
	{
		Curse::Position op = curse.getCursorPosition();

		curse.moveCursor(0, Screen::instance().getScreenHeight() - 1);
		curse.putch(':');

		std::string cmd = getline();

		Screen::instance().eraseLine();

		curse.moveCursor(op.x, op.y);

		return cmd;
	}

	Keyboard& Keyboard::instance(void)
	{
		static Keyboard kb;
		return kb;
	}

	Keyboard::Keyboard(void) : curse(Curse::instance())
	{
	}

	Keyboard::~Keyboard(void)
	{
	}
}

