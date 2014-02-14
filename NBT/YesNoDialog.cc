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

		getCursorPos(ox, oy);

		int w, h, x1, x2, y1, y2;
		getScreenSize(w, h);
		x1 = w / 3;     y1 = h / 3;
		x2 = 2 * w / 3; y2 = 2 * h / 3;

		drawBorder<Borders::DOUBLE>(x1, y1, x2, y2);

		setCursorPos(x1 + 4, y1 + 2);
		printf(msg.c_str());

		int ly = strlen(YES), ln = strlen(NO);

		int c1 = x1 + (    (x2 - x1) / 2 - ln) / 2;
		int c2 = x1 + (3 * (x2 - x1) / 2 - ly) / 2;

		drawNo = [this, c1, y2](bool selected)
			{
				setCursorPos(c1, y2 - 4);
				printf(NO);
			};
		drawYes = [this, c2, y2](bool selected)
			{
				setCursorPos(c2, y2 - 4);
				printf(YES);
			};

		drawYes(false);
		drawNo(false);
	}

	void YesNoDialog::input(int ch)
	{
		switch(ch)
		{
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
			default:
				quit();
				break;
		}
	}

	void YesNoDialog::update(int ms)
	{
	}

	void YesNoDialog::refresh(void)
	{
	}

	void YesNoDialog::finalize(void)
	{
		if(ox >= 0 && oy >= 0) setCursorPos(ox, oy);
	}
}

