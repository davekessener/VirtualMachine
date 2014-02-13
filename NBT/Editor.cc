#include "Editor.h"

Editor::Editor(void) : b("-", 10, 3, 4, -1, 0)
{
}

Editor::~Editor(void)
{
}

void Editor::input(int ch)
{
	int w, h;
	getScreenSize(w, h);

	switch(ch)
	{
		case 'q':
			quit();
			break;
		case ':':
			showCursor(true);
			setCursorPos(0, w - 1);
			clearLine();
			printf(":");
			setInputFunction(ncurses::ReadLine(*this, [this](const std::string& s) 
				{
					b.setMsg(s, 0);
					this->setCursorPos(0, 0);
					this->showCursor(false);
				}));
			break;
	}
}

void Editor::update(int ms)
{
	b.update(ms);
}

void Editor::refresh(void)
{
	b.draw(*this);
}

