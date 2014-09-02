#include "Menu.h"
#include <algorithm>
#include <dav/gl.h>
#include <dav/Logger.h>
#include "../Text.h"

#define MXT_BARH (Text::C_W * 3 / 2)

namespace editor { namespace surface
{
	namespace
	{
		inline int slen(const std::string& s)
		{
			return (s.length() + 1) * Text::C_W;
		}
	}

	class MenuItem : public Surface
	{
		typedef Menu::menu_fn menu_fn;
		typedef Menu::enable_fn enable_fn;
		typedef Menu::item_map item_map;
		using Surface::MouseButtons;

		public:
			template<typename I>
				MenuItem(I i1, I i2) : v_(i1, i2), idx_(-1)
				{
					const std::string *s = nullptr;
					for(const auto& v : v_)
					{
						if(!s || v.id.length() > s->length()) s = &v.id;
					}
					mw_ = slen(*s);
				}
			inline int maxWidth( ) const { return mw_; }
		private:
			void i_doInit( ) { }
			void i_doUpdate(int);
			void i_doRender( ) const;
			void i_doMouseOver(bool);
			void i_doMouseMove(int, int);
			void i_doMouseUp(MouseButtons, int, int);
			inline int getIdxFromY(int y) const { return y / MXT_BARH; }
		private:
			item_map v_;
			int mw_, idx_;
	};

	namespace
	{
		inline MenuItem& toSMI(Surface_ptr p)
		{
			return *dynamic_cast<MenuItem *>(&*p);
		}
	}

// # ===========================================================================

	void Menu::i_doInit(void)
	{
		int x = 0;
		for(auto& m : menudata_)
		{
			Surface_ptr p(new MenuItem(m.second.begin(), m.second.end()));
			p->init(x, MXT_BARH, toSMI(p).maxWidth(), m.second.size() * MXT_BARH);
			x += slen(m.first);
			menus_.push_back(p);
		}

		idx_ = -1;
	}

	void Menu::i_doUpdate(int d)
	{
		if(curMenu_)
		{
			if(curMenu_->hidden())
			{
				removeCurMenu();
			}
		}
	}

	void Menu::i_doRender(void) const
	{
		point p(getAbsCoords()), q(p.x + width(), p.y + height());

		dav::gl::draw_rect(p.x, p.y, q.x, q.y, 0xffffff);
		dav::gl::draw_rect(p.x, p.y, q.x, p.y + MXT_BARH, 0xc0c0c0);

		int x = 0, i = 0;
		for(const auto& m : menudata_)
		{
			int w = slen(m.first);
			
			if(idx_ == i)
			{
				dav::gl::draw_rect(p.x + x, p.y, p.x + x + w, p.y + MXT_BARH, 0x404040);
			}

			Text::drawText(p.x + x + Text::C_W / 2, p.y + Text::C_W / 4, m.first, idx_ == i ? 0xffffff : 0);

			x += w;
			++i;
		}
	}
	
	void Menu::i_doMouseOver(bool over)
	{
		if(!over)
		{
			idx_ = -1;
			dirty();
		}
	}

	void Menu::i_doMouseMove(int x, int y)
	{
		if(x < 0 || x >= (long)width() || y < 0 || y >= MXT_BARH)
		{
			removeCurMenu();
			return;
		}

		int idx = getIdxFromX(x);
		bool menuOpen = static_cast<bool>(curMenu_);

		if(curMenu_ && idx < 0)
		{
			removeCurMenu();
		}

		if(idx_ != idx)
		{
			idx_ = idx;
			
			removeCurMenu();
			
			if(idx_ > 0 && menuOpen)
			{
				addChild(curMenu_ = menus_.at(idx_));
				toSMI(curMenu_).show();
			}

			dirty();
		}
	}

	void Menu::i_doMouseUp(MouseButtons b, int x, int y)
	{
		LOG("Registered mouse up @%d,%d", x, y);

		if(b != MouseButtons::LEFT || x < 0 || x >= (int)width() || y < 0 || y >= MXT_BARH)
		{
			removeCurMenu();
			return;
		}
		
		if((idx_ = getIdxFromX(x)) < 0)
		{
			removeCurMenu();
			return;
		}

		LOG("> Opened menu %d", idx_);

		if(curMenu_) removeChild(curMenu_->ID());
		addChild(curMenu_ = menus_.at(idx_));
		toSMI(curMenu_).show();
		dirty();
	}

// # ---------------------------------------------------------------------------

	void Menu::setContent(Surface_ptr p)
	{
		addChild(p);
		p->init(0, MXT_BARH, width(), height() - MXT_BARH);
	}

	void Menu::addItem(const std::string& menu, const std::string& item, menu_fn fn, enable_fn en)
	{
		auto i = std::find_if(menudata_.begin(), menudata_.end(), [&menu](menu_map::value_type& p)
			{ return p.first == menu; });

		if(i == menudata_.end())
		{
			menudata_.push_back(std::make_pair(menu, item_map()));
			i = menudata_.end();
			--i;
		}

		i->second.push_back(Item{item, fn, en, true});
	}

// # ---------------------------------------------------------------------------
	
	int Menu::getIdxFromX(int x) const
	{
		int idx = -1, l = 0;
		do
		{
			x -= l;
			++idx;
			
			if(idx >= (long)menudata_.size()) return -1;
		
			l = slen(menudata_.at(idx).first);
		} while(l < x);
		return idx;
	}

// # ===========================================================================

	void MenuItem::i_doMouseOver(bool over)
	{
		if(!over)
		{
			idx_ = -1;
			hide();
			dirty();
		}
	}

	void MenuItem::i_doMouseMove(int x, int y)
	{
		if(x < 0 || x >= (long)width() || y < 0 || y >= (long)height()) return;

		int idx = getIdxFromY(y);

		if(idx != idx_)
		{
			idx_ = idx;
			dirty();
		}
	}

	void MenuItem::i_doMouseUp(MouseButtons b, int x, int y)
	{
		if(x < 0 || x >= (long)width() || y < 0 || y >= (long)height()) return;

		int i = getIdxFromY(y);

		if(i < 0) return;

		auto& mi(v_.at(i));

		if(mi.enabled)
		{
			mi.activate();
			idx_ = -1;
			hide();
			dirty();
		}
	}

	void MenuItem::i_doUpdate(int d)
	{
		for(auto& i : v_)
		{
			if(static_cast<bool>(i.enable))
			{
				bool f = i.enable();
				if(f != i.enabled)
				{
					i.enabled = f;
					dirty();
				}
			}
		}
	}

	void MenuItem::i_doRender(void) const
	{
		point o(getAbsCoords()), q(o.x + width(), o.y + height());

		dav::gl::draw_rect(o.x, o.y, q.x, q.y, 0xffffff);

		int y = o.y, i = 0;
		for(const auto& p : v_)
		{
			int tc = p.enabled ? 0 : 0xa0a0a0;

			if(idx_ == i && p.enabled)
			{
				dav::gl::draw_rect(o.x, y, q.x, y + MXT_BARH, 0x404040);
				tc = 0xffffff;
			}

			Text::drawText(o.x + Text::C_W / 2, y + Text::C_W / 4, p.id, tc);

			y += MXT_BARH;
			++i;
		}
	}
}}

