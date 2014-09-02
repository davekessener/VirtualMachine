#include "FileSelect.h"
#include <boost/filesystem.hpp>
#include "../Text.h"
#include "Button.h"
#include "StringList.h"
#include "StringInput.h"
#include <dav/gl.h>
#include <dav/Logger.h>

#define MXT_BARH (Text::C_W * 2)

namespace editor { namespace surface { 

void FileSelect::i_doInit(void)
{
	dir_ = boost::filesystem::absolute("").string() + "/";
	LOG("Absolute path: %s", dir_.data());
	list_.reset(new StringList([this](const std::string& s)
		{
			LOG("Received string '%s' from SL!", s.data());
			if(boost::filesystem::is_directory(dir_ + s))
			{
				dynamic_cast<StringInput *>(&*input_)->setInput("");
				dir_ += s;
				if(dir_.back() != '/') dir_.push_back('/');
				dirty();
			}
			else
			{
				dynamic_cast<StringInput *>(&*input_)->setInput(s);
			}
		}));
	list_->init(0, MXT_BARH, width(), height() - 2 * MXT_BARH);
	addChild(list_);

	input_.reset(new StringInput([this](void)
		{
			hide();
			select_(dir_ + dynamic_cast<StringInput *>(&*input_)->getInput());
		}));
	input_->init(0, height() - MXT_BARH, width() - 5 * Text::C_W / 2, MXT_BARH);
	addChild(input_);

	Surface_ptr p(new Button(std::string(1, (char)255), [this](void)
		{
			auto i = dir_.find_last_of('/', dir_.length() - 2);
			if(i && i != std::string::npos)
			{
				dir_ = dir_.substr(0, i) + "/";
				dynamic_cast<StringInput *>(&*input_)->setInput("");
				dirty();
			}
		}));
	p->init(0, 0, MXT_BARH, MXT_BARH);
	addChild(p);

	p.reset(new Button("X", std::bind(&Surface::hide, this)));
	p->init(width() - MXT_BARH, 0, MXT_BARH, MXT_BARH);
	addChild(p);

	p.reset(new Button("OK", [this](void)
		{
			hide();
			select_(dir_ + dynamic_cast<StringInput *>(&*input_)->getInput());
		}));
	p->init(width() - 5 * Text::C_W / 2, height() - MXT_BARH, 5 * Text::C_W / 2, MXT_BARH);
	addChild(p);
}

void FileSelect::i_doUpdate(int d)
{
	const std::string &s(dynamic_cast<StringInput *>(&*input_)->getInput());

	if(s != cur_)
	{
		cur_ = s;
		dirty();
	}
}

void FileSelect::i_doPrerender(void)
{
	using namespace boost::filesystem;

	svec_t files;

	for(directory_iterator i(dir_), e ; i != e ; ++i)
	{
		if(is_regular_file(*i) || is_directory(*i))
		{
			std::string fn(static_cast<path>(*i).string());
			fn = fn.substr(fn.find_last_of('/') + 1);

			if(cur_.empty() || (fn.length() >= cur_.length() && cur_ == fn.substr(0, cur_.length())))
			{
				if(is_directory(*i)) fn.push_back('/');
				files.push_back(fn);
			}
		}
	}

	dynamic_cast<StringList *>(&*list_)->load(files.cbegin(), files.cend());
}

void FileSelect::i_doRender(void) const
{
	point p(getAbsCoords()), q(p.x + width(), p.y + height());

	dav::gl::draw_rect(p.x, p.y, q.x, q.y, 0xc0c0c0);
	Text::drawText(p.x + MXT_BARH + Text::C_W / 2, p.y + MXT_BARH / 2 - Text::C_W / 2, dir_);
}

void FileSelect::i_doMouseUp(MouseButtons b, int x, int y)
{
	LOG("FS MouseUp @%d,%d", x, y);
}

void FileSelect::i_doKeyDown(Controls key)
{
}

}}

