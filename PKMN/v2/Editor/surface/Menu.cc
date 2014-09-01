#include "Menu.h"
#include <algorithm>
#include <dav/gl.h>
#include <dav/Logger.h>
#include "../Text.h"

#define MXT_BARH (Text::C_W * 3 / 2)

namespace editor { namespace surface
{
	class MenuItem : public Surface
	{
		typedef std::function<void(void)> menu_fn;
		typedef std::vector<std::pair<std::string, menu_fn>> vec_t;
		using Surface::MouseButtons;

		public:
			template<typename I>
				MenuItem(I i1, I i2) : v_(i1, i2)
				{
					size_t ml = 0;
					for(const auto& v : v_)
					{
						if(v.first.length() > ml) ml = v.first.length();
					}
					mw_ = ((long)ml + 1) * Text::C_W;
				}
			inline int maxWidth( ) const { return mw_; }
		private:
			void i_doInit( ) { }
			void i_doUpdate(int) { }
			void i_doRender( ) const;
			void i_doMouseUp(MouseButtons, int, int);
		private:
			vec_t v_;
			int mw_;
	};

	namespace
	{
		MenuItem& toSMI(Surface_ptr p)
		{
			return *dynamic_cast<MenuItem *>(&*p);
		}
	}

	void Menu::setContent(Surface_ptr p)
	{
		addChild(p);
		p->init(0, MXT_BARH, width(), height() - MXT_BARH);
	}

	void Menu::i_doInit(void)
	{
		int x = 0;
		for(auto& m : menudata_)
		{
			Surface_ptr p(new MenuItem(m.second.begin(), m.second.end()));
			p->init(x, MXT_BARH, toSMI(p).maxWidth(), m.second.size() * MXT_BARH);
			x += (m.first.length() + 1) * Text::C_W;
			menus_.push_back(p);
		}
	}

	void Menu::i_doUpdate(int d)
	{
		if(curMenu_ && curMenu_->hidden())
		{
			removeChild(curMenu_->ID());
			dirty();
		}
	}

	void Menu::i_doRender(void) const
	{
		point p(getAbsCoords());

		dav::gl::draw_rect(p.x, p.y, p.x + width(), p.y + MXT_BARH, 0xffffff);

		int x = Text::C_W / 2;
		for(const auto& m : menudata_)
		{
			Text::drawText(x + p.x, Text::C_W / 4 + p.y, m.first);
			x += (m.first.length() + 1) * Text::C_W;
		}
	}

	void Menu::i_doMouseUp(MouseButtons b, int x, int y)
	{
		LOG("Registered mouse up @%d,%d", x, y);
		auto leave = [this]( ) { if(curMenu_) { removeChild(curMenu_->ID()); curMenu_.reset(); dirty(); } };

		if(b != MouseButtons::LEFT || x < 0 || x >= (int)width() || y < 0 || y >= MXT_BARH)
		{
			leave();
			return;
		}
		
		int idx = -1, l = 0;
		do
		{
			x -= l;
			++idx;
			
			if(idx >= (long)menudata_.size())
			{
				leave();
				return;
			}
		
			l = (menudata_.at(idx).first.length() + 1) * Text::C_W;
		} while(l < x);

		LOG("> Opened menu %d", idx);

		if(curMenu_) removeChild(curMenu_->ID());
		addChild(curMenu_ = menus_.at(idx));
		toSMI(curMenu_).show();
		dirty();
	}

	void Menu::addItem(const std::string& menu, const std::string& item, menu_fn fn)
	{
		auto i = std::find_if(menudata_.begin(), menudata_.end(), [&menu](menu_map::value_type& p)
			{ return p.first == menu; });

		if(i == menudata_.end())
		{
			menudata_.push_back(std::make_pair(menu, item_map()));
			i = menudata_.end();
			--i;
		}

		i->second.push_back(std::make_pair(item, fn));
	}

// # ===========================================================================

	void MenuItem::i_doMouseUp(MouseButtons b, int x, int y)
	{
		int i = y / MXT_BARH;
		LOG("Item click @%d,%d: item %d", x, y, i);
		assert(i>=0&&i<(long)v_.size());
		LOG("Clicked '%s'", v_.at(i).first.data());
		v_.at(i).second();
		hide();
		dirty();
	}

	void MenuItem::i_doRender(void) const
	{
		point o(getAbsCoords());

		dav::gl::draw_rect(o.x, o.y, o.x + width(), o.y + height(), 0xffffff);

		int y = 0;
		for(const auto& p : v_)
		{
			Text::drawText(Text::C_W / 2 + o.x, o.y + y++ * MXT_BARH + Text::C_W / 4, p.first);
		}
	}
}}

