#define CURSE_YESNODIALOG_MAIN
#include "YesNoDialog.h"
#undef CURSE_YESNODIALOG_MAIN

namespace ncurses
{	
	const char *YesNoDialog::YES = " Yes";
	const char *YesNoDialog::NO  = " No ";

	YesNoDialog::YesNoDialog(const std::string& _msg, answerFn _answer, Choices _select)
		: msg(_msg), answer(_answer), select(_select), ox(-1), oy(-1)
	{
	}

	YesNoDialog::~YesNoDialog(void)
	{
	}

	void YesNoDialog::init(void)
	{
		Curse::init();
		showCursor(false);

		getCursorPos(ox, oy);

		int w, h, x1, x2, y1, y2;
		getScreenSize(w, h);
		int lc = (msg.length() / (w - 4)) + 1;
		int tl = (msg.length() / lc + 1) & ~1;

		int wh = 12 + lc, ww = (int) (wh * (w / (double)h) + 0.5);
		x1 = (w - ww) / 2; y1 = (h - wh) / 2;
		x2 = (w + ww) / 2; y2 = (h + wh) / 2;

		drawBorder<Borders::BOLD_SINGLE>(x1, y1, x2, y2);

		for(int y = y1 + (y2 - y1) / 2 - 3 - lc / 2, l = 0 ; l < lc ; ++l, ++y)
		{
			setCursorPos(x1 + ((x2 - x1) - tl) / 2, y);
			printf(msg.substr(l * tl, l == lc - 1 ? -1 : (l + 1) * tl).c_str());
		}

		int ly = strlen(YES), ln = strlen(NO);

		int c1 = x1 + (    (x2 - x1 + 1) / 2 - ln + 1) / 2;
		int c2 = c1 + (x2 - x1) / 2 - 1;

		drawNo = [this, c1, x1, y1, x2, y2, ln](bool selected)
			{
				if(selected)
				{
					drawBorder<Borders::DOUBLE>(x1 + 2, y2 - 6, x1 + (x2 - x1) / 2 - 1, y2 - 1);
				}
				else
				{
					clearRect(x1 + 2, y2 - 6, x1 + (x2 - x1) / 2 - 1, y2 - 1);
				}
				setCursorPos(c1, y2 - 4);
				printf(NO);
				setCursorPos(c1 + 1, y2 - 4);
				changeAttribute(Attributes::UNDERSCORE);
			};
		drawYes = [this, c2, x1, y1, x2, y2, ly](bool selected)
			{
				if(selected)
				{
				drawBorder<Borders::DOUBLE>(x1 + (x2 - x1) / 2 + 1, y2 - 6, x2 - 2, y2 - 1);
				}
				else
				{
					clearRect(x1 + (x2 - x1) / 2 + 1, y2 - 6, x2 - 2, y2 - 1);
				}
				setCursorPos(c2, y2 - 4);
				printf(YES);
				setCursorPos(c2 + 1, y2 - 4);
				changeAttribute(Attributes::UNDERSCORE);
			};

		clean = [this, x1, y1, x2, y2] { clearRect(x1, y1, x2, y2); };

		drawYes(false);
		drawNo(false);
	}

	void YesNoDialog::input(int ch)
	{
		switch(ch)
		{
			case 'Y':
			case 'y':
				select = Choices::YES;
				input(Keys::ENTER);
				break;
			case 'N':
			case 'n':
				select = Choices::NO;
				input(Keys::ENTER);
				break;
			case Keys::LEFT:
				switch(select)
				{
					case Choices::NONE:
						select = Choices::NO;
						break;
					case Choices::YES:
						select = Choices::NO;
						break;
				}
				break;
			case Keys::RIGHT:
				switch(select)
				{
					case Choices::NONE:
						select = Choices::YES;
						break;
					case Choices::NO:
						select = Choices::YES;
						break;
				}
				break;
			case Keys::ESCAPE:
				select = Choices::NO;
			case Keys::ENTER:
				if(select != Choices::NONE)
				{
					answer(select == Choices::YES);
					quit();
				}
				break;
		}
	}

	void YesNoDialog::update(int ms)
	{
	}

	void YesNoDialog::refresh(void)
	{
		drawYes(select == Choices::YES);
		drawNo(select == Choices::NO);
	}

	void YesNoDialog::finalize(void)
	{
		if(static_cast<bool>(clean)) clean();
		if(ox >= 0 && oy >= 0) setCursorPos(ox, oy);
		select = Choices::NONE;
	}
}

