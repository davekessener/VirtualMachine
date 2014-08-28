#include <vector>
#include <cassert>
#include "EnterString.h"
#include "Terminal.h"
#include <dav/Logger.h>
#include <dav/utils.h>

EnterString::EnterString(int w, int h, enter_fn enter, const std::string& prompt, const std::string& input)
	: x1_(w / 4), y1_(h / 2 - 4), x2_(3 * w / 4), y2_(h / 2 + 3), enter_(enter), input_(input), prompt_(prompt)
{
	typedef display::BorderTypes<display::Borders::DOUBLE> Border;

	assert(x1_>=0&&y1_>=0&&x2_>x1_&&y2_>y1_);

	cpos_ = input_.length();

	std::vector<wchar_t>(x2_ - x1_, wchar_t(Border::SIDES_HORIZONTAL)).swap(horz_);
	std::vector<wchar_t>(x2_ - x1_, L' ').swap(vert_);
	vert_.front() = vert_.back() = Border::SIDES_VERTICAL;
}

void EnterString::i_doRender(void) const
{
	display::Terminal &t(display::Terminal::instance());

	drawBorder();
	t.setCursorPos(x1_ + 3, y1_ + 2);
	t.printf("%s", prompt_.data());
	t.setCursorPos(x1_ + 3, y1_ + 4);
	t.printf("%s", input_.data());
	t.setCursorPos(x1_ + 3 + cpos_, y1_ + 4);
}

void EnterString::i_doInput(int in)
{
	Control_ptr self = shared_from_this(); // <-- IMPORTANT!

	switch(in)
	{
		case display::Keys::ENTER:
			popControl();
			enter_(input_);
			break;
		case display::Keys::LEFT:
			if(cpos_ > 0) --cpos_;
			break;
		case display::Keys::RIGHT:
			if(cpos_ < input_.length()) ++cpos_;
			break;
		case display::Keys::UP:
			cpos_ = 0;
			break;
		case display::Keys::DOWN:
			cpos_ = input_.length();
			break;
		case display::Keys::BACKSPACE:
			if(cpos_ > 0 && !input_.empty())
			{
				input_.erase(input_.begin() + cpos_ - 1);
				--cpos_;
			}
			break;
		default:
			if((in >= 'a' && in <= 'z') || (in >= 'A' && in <= 'Z') || (in >= '0' && in <= '9') ||
				dav::utils::isIn(dav::utils::non_alphanum, in))
			{
				input_.insert(input_.begin() + cpos_, in);
				++cpos_;
			}
			break;
	}
}

void EnterString::drawBorder(void) const
{
	typedef display::BorderTypes<display::Borders::DOUBLE> Border;
	display::Terminal &t(display::Terminal::instance());

	for(int y = y1_ ; y < y2_ ; ++y)
	{
		t.setCursorPos(x1_, y);
		t.printfw((y == y1_ || y == y2_ - 1) ? &*horz_.begin() : &*vert_.begin());
	}

	t.setCursorPos(x1_, y1_);
	t.printfw(L"%lc", Border::CORNER_TOPLEFT);
	t.setCursorPos(x2_ - 1, y1_);
	t.printfw(L"%lc", Border::CORNER_TOPRIGHT);
	t.setCursorPos(x1_, y2_ - 1);
	t.printfw(L"%lc", Border::CORNER_BOTTOMLEFT);
	t.setCursorPos(x2_ - 1, y2_ - 1);
	t.printfw(L"%lc", Border::CORNER_BOTTOMRIGHT);
}

