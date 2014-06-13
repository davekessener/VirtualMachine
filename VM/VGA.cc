#include "VGA.h"
#include "Logger.h"
#include "CPU.h"
#include "Screen.h"
#include <cassert>

#define MXT_P 4

namespace vm { namespace periphery {

using sdl::Image;
using sdl::color_rgba_t;
using cpu::CPU;
using sdl::Screen;

BasicVGA::BasicVGA(void) : tex_(), vidmem_(NULL)
{
	cursor_.x = cursor_.y = 0;
	cursor_.active = false;
	cursor_.show = cursor_.show_old = true;

	std::uint32_t p[] = {0x000000ff, 0xff0000ff, 0x00ff00ff, 0x0000ffff,
				0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 
				0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 
				0xffff00ff, 0x00ffffff, 0xff00ffff, 0xffffffff};
	
	for(int i = 0 ; i < 16 ; ++i) palette_[i] = p[i];

	Screen::instance().aquire("CPU", COLS * CHAR_SIZE, ROWS * (CHAR_SIZE + MXT_P));

	tex_.create(COLS * CHAR_SIZE, ROWS * (CHAR_SIZE + MXT_P));
}

void BasicVGA::link(CPU& cpu)
{
	Periphery::link(cpu);

	Screen::instance().onQuit(std::bind(&CPU::halt, &cpu));
}

void BasicVGA::in(WORD w)
{
	queue_.push_back(w);

	switch(queue_.front())
	{
		case 0x01:
			if(queue_.size() >= 2)
			{
				queue_.pop_front();
				loadFrom(queue_.front());
				queue_.pop_front();
			}
			break;
		case 0x02:
			if(queue_.size() >= 3)
			{
				queue_.pop_front();
				cursor_.x = queue_.front();
				queue_.pop_front();
				cursor_.y = queue_.front();
				queue_.pop_front();
			}
			break;
		case 0x03:
			if(queue_.size() >= 2)
			{
				queue_.pop_front();
				cursor_.active = queue_.front();
				queue_.pop_front();
			}
			break;
		case 0x04:
			queue_.pop_front();
			if(++cursor_.x >= COLS)
			{
				cursor_.x = 0;
				if(++cursor_.y >= ROWS) cursor_.y = ROWS - 1;
			}
			break;
		default:
			LOG("Invalid command! [0x%04x]", queue_.front());
			queue_.pop_front();
			break;
	}
}

WORD BasicVGA::out(void)
{
	return 0;
}

void BasicVGA::step(void)
{
	if(t_.elapsed() > BLINK_STEP)
	{
		cursor_.show = !cursor_.show;
		t_.reset();
		refresh();
		cursor_.show_old = cursor_.show;
	}
}

void BasicVGA::loadFrom(ADDRESS a)
{
	if(a + SCREEN_SIZE > static_cast<ADDRESS>(-1))
	{
		LOG("ERR: Invalid address for video memory: @0x%04x", a);
		return;
	}

	vidmem_ = cpu_->RAM() + a;
	refresh();
}

void BasicVGA::refresh(bool force)
{
	const WORD *n = vidmem_ ? vidmem_ : buffer_;
	WORD *b = buffer_;

	tex_.startBlit();

	for(size_t y = 0 ; y < ROWS ; ++y)
	{
		for(size_t x = 0 ; x < COLS ; ++x)
		{
			bool invert = cursor_.active &&
				(x == cursor_.x && y == cursor_.y && cursor_.show != cursor_.show_old);

			if(force || invert || *n != *b)
			{
				WORD fc = 0x0f - ((*n >> 12) & 0x0f);
				WORD bc = (*n >>  8) & 0x0f;
				char c  = static_cast<char>(*n & 0xff);

				color_rgba_t fgc((invert && cursor_.show) ? palette_[fc].invert() : palette_[fc]);
				color_rgba_t bgc((invert && cursor_.show) ? palette_[bc].invert() : palette_[bc]);

				if(bc != 0 || invert)
				{
					tex_.fillRect(bgc, x * CHAR_SIZE, y * (CHAR_SIZE + MXT_P), CHAR_SIZE, CHAR_SIZE + MXT_P);
				}

				tex_.drawChar(fgc, x * CHAR_SIZE, y * (CHAR_SIZE + MXT_P) + MXT_P / 2, c);

				if(*b != *n) *b = *n;
			}

			n++;
			b++;
		}
	}

	tex_.stopBlit();

	tex_.render();
}

}}

