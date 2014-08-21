#include "ViewBuffer.h"
#include "Terminal.h"

namespace
{
	ViewBuffer::vec_t optimise(const ViewBuffer::vec_t&, int, int, int, int);
}

void ViewBuffer::renderAt(int tx, int ty, int sx, int sy, int w, int h) const
{
	display::Terminal &t(display::Terminal::instance());

	if(sx != sx_ || sy != sy_ || w != w_ || h != h_)
	{
		opt_ = optimise(vec_, sx_ = sx, sy_ = sy, w_ = w, h_ = h);
	}

	for(const auto& l : opt_)
	{
		t.setCursorPos(tx - sx + l.pos.x, ty - sy + l.pos.y);
		t.printf("%s", l.line.data());
	}
}

namespace
{
	ViewBuffer::line_t process(const ViewBuffer::line_t& line, int sx, int w)
	{
		std::string s(line.line);
		if(line.pos.x < sx) s = s.substr(sx - line.pos.x);
		if((long)s.length() > w) s = s.substr(0, w - 1);
		
		ViewBuffer::line_t l;
		l.pos.x = line.pos.x < sx ? sx : line.pos.x;
		l.pos.y = line.pos.y;
		l.line  = s;

		return l;
	}

	ViewBuffer::vec_t optimise(const ViewBuffer::vec_t& vec, int x, int y, int w, int h)
	{
		ViewBuffer::vec_t v;
		v.reserve(vec.size());

		for(const auto& l : vec)
		{
			if(l.pos.y >= y && l.pos.y < y + h)
			{
				auto ll(process(l, x, w));
				if(!ll.line.empty()) v.push_back(ll);
			}
		}

		return v;
	}
}

