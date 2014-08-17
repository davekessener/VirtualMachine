#include <cassert>
#include "VGA.h"
#include <dav/Logger.h>
#include "CPU.h"
#include "Screen.h"
#include <dav/gl.h>
#include <png/png.hpp>

#define MXT_CHARSET "./charset.png"

using namespace dav;

namespace vm { namespace periphery {

//using sdl::Image;
//using sdl::color_rgba_t;
using cpu::CPU;
using sdl::Screen;

using namespace vm::vga;

BasicVGA::BasicVGA(void) : /*tex_(),*/ vidmem_(NULL)
{
	cursor_.x = cursor_.y = 0;
	cursor_.active = false;
	cursor_.show = cursor_.show_old = true;

	std::uint32_t p[] = {0x000000ff, 0xff0000ff, 0x00ff00ff, 0x0000ffff,
				0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 
				0x000000ff, 0x000000ff, 0x000000ff, 0x000000ff, 
				0xffff00ff, 0x00ffffff, 0xff00ffff, 0xffffffff};
	
	for(int i = 0 ; i < 16 ; ++i)
	{
		palette_[i] = p[i];
#define INV(c) ((((c)&0xff0000)>>16)|((c)&0x00ff00)|(((c)&0x0000ff)<<16))
		ipalette_[i] = INV(p[i]);
#undef INV
	}

	{
		png::image<png::rgba_pixel> img(MXT_CHARSET);
		const int w = img.get_width(), h = img.get_height();
		BYTE *buf = new BYTE[w * h * 3];

		for(int y = 0 ; y < h ; ++y)
		{
			for(int x = 0 ; x < w ; ++x)
			{
				auto c = img.get_pixel(x, y);
				buf[(x + y * w) * 3 + 0] = c.red;
				buf[(x + y * w) * 3 + 1] = c.green;
				buf[(x + y * w) * 3 + 2] = c.blue;
			}
		}
		
		tex_ = gl::create_texture(buf, w, h);

		delete[] buf;
	}

	Screen::instance().addRender(std::bind(&BasicVGA::refresh, this));
//	tex_.create(COLS * CHAR_SIZE, ROWS * (CHAR_SIZE + MXT_P));
}

void BasicVGA::link(CPU& cpu)
{
	Periphery::link(cpu);

//	Screen::instance().onQuit(std::bind(&CPU::halt, &cpu));
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
//		refresh();
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
//	refresh();
}

void BasicVGA::refresh(void) const
{
	if(!vidmem_) return;
	const WORD *b = vidmem_;

	for(size_t y = 0 ; y < ROWS ; ++y)
	{
		for(size_t x = 0 ; x < COLS ; ++x)
		{
			WORD c = *b++;
			float u = (c % 16) * 0.0625, v = (c / 16) * 0.0625;
			int tx = x * CHAR_SIZE, ty = y * (CHAR_SIZE + 4);
			bool invert = cursor_.active && (x == cursor_.x && y == cursor_.y) && cursor_.show;
			
			int bg = (c >> 8) & 0x0f, fg = 0x0f - ((c >> 12) & 0x0f);
			
			DWORD fgc = invert && cursor_.show ? ipalette_[fg] : palette_[fg];
			DWORD bgc = invert && cursor_.show ? ipalette_[bg] : palette_[bg];

			gl::draw_rect(tx, ty, tx + CHAR_SIZE, ty + CHAR_SIZE + 4, bgc);
			gl::color(fgc);
			gl::draw_face2d(u, v, u + 0.0625, v + 0.0625, tx, ty, tx + CHAR_SIZE, ty + CHAR_SIZE + 2);
			gl::color(1.0, 1.0, 1.0);
		}
	}
//	const WORD *n = vidmem_ ? vidmem_ : buffer_;
//	WORD *b = buffer_;
//
//	tex_.startBlit();
//
//	for(size_t y = 0 ; y < ROWS ; ++y)
//	{
//		for(size_t x = 0 ; x < COLS ; ++x)
//		{
//			bool invert = cursor_.active &&
//				(x == cursor_.x && y == cursor_.y && cursor_.show != cursor_.show_old);
//
//			if(force || invert || *n != *b)
//			{
//				WORD fc = 0x0f - ((*n >> 12) & 0x0f);
//				WORD bc = (*n >>  8) & 0x0f;
//				char c  = static_cast<char>(*n & 0xff);
//
//				color_rgba_t fgc((invert && cursor_.show) ? palette_[fc].invert() : palette_[fc]);
//				color_rgba_t bgc((invert && cursor_.show) ? palette_[bc].invert() : palette_[bc]);
//
//				if(bc != 0 || invert || *n != *b)
//				{
//					tex_.fillRect(bgc, x * CHAR_SIZE, y * (CHAR_SIZE + MXT_P), CHAR_SIZE, CHAR_SIZE + MXT_P);
//				}
//
//				tex_.drawChar(fgc, x * CHAR_SIZE, y * (CHAR_SIZE + MXT_P) + MXT_P / 2, c);
//
//				if(*b != *n) *b = *n;
//			}
//
//			n++;
//			b++;
//		}
//	}
//
//	tex_.stopBlit();
//
//	tex_.render();
}

}}

