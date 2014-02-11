#include "Editor.h"

int Editor::run(const std::vector<std::string>& args)
{
	ncurses::Curse::instance().begin();

	ncurses::Screen &s = ncurses::Screen::instance();
	ncurses::Keyboard &kb = ncurses::Keyboard::instance();

	ncurses::Curse::instance().setBufferMode(ncurses::CharBufferMode::CBREAK);
	ncurses::Curse::instance().setBufferAttribute(ncurses::BufferAttribute::ECHO, ncurses::SetMode::RESET);
	ncurses::Curse::instance().setBufferAttribute(ncurses::BufferAttribute::SHOW_CURSOR, ncurses::SetMode::RESET);
	ncurses::Curse::instance().setTimeout(10);

	ncurses::Banner b(std::string("Hi there, handsome!"), 10, 1, 200);

	wchar_t ch;
	while((ch = ncurses::Curse::instance().getch()) != 'q')
	{
		b.update();
		b.draw();
		s.refresh();

		switch(ch)
		{
			case ncurses::Curse::ErrChar:
				continue;
			case ':':
				b.setMsg(kb.readCommand());
				break;
		}
	}

	return 0;
}

