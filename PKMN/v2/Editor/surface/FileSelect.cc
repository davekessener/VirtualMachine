#include "FileSelect.h"
#include <boost/filesystem.hpp>
#include "../Text.h"
#include "Button.h"
#include "StringList.h"
#include <dav/gl.h>
#include <dav/Logger.h>

#define MXT_BARH (Text::C_W * 3 / 2)

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
				cur_ = "";
				idx_ = 0;
				dir_ += s;
				if(dir_.back() != '/') dir_.push_back('/');
				dirty();
			}
			else
			{
				cur_ = s;
				idx_ = s.size();
			}
		}));
	list_->init(0, MXT_BARH, width(), height() - 2 * MXT_BARH);
	addChild(list_);

	Surface_ptr p(new Button(std::string(1, (char)255), [this](void)
		{
			LOG("Received 'dir up' command.");
			auto i = dir_.find_last_of('/', dir_.length() - 2);
			if(i && i != std::string::npos)
			{
				dir_ = dir_.substr(0, i) + "/";
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
		}));
	p->init(width() - 5 * Text::C_W / 2, height() - MXT_BARH, 5 * Text::C_W / 2, MXT_BARH);
	addChild(p);
}

void FileSelect::i_doUpdate(int d)
{
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

			if(cur_.empty() || (fn.length() <= cur_.length() && fn == cur_.substr(0, fn.length())))
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

	dav::gl::draw_rect(p.x, p.y, q.x, q.y, 0xcccccc);
	Text::drawText(p.x + MXT_BARH + 4, p.y + Text::C_W / 4, dir_);
}

void FileSelect::i_doMouseUp(MouseButtons b, int x, int y)
{
	LOG("FS MouseUp @%d,%d", x, y);
}

void FileSelect::i_doKeyDown(Controls key)
{
}

}}

