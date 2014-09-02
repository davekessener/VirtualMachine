#include "Surface.h"
#include <map>
#include <dav/Logger.h>

namespace editor { namespace surface
{
	void Surface::render(void) const
	{
		i_doRender();

		for(auto i = children_.cend(), b = children_.cbegin() ; i != b ;)
		{
			(*--i)->render();
		}
	}

	void Surface::keyDown(Controls key)
	{
		LOG("Key %u(%c) is pressed.", static_cast<DWORD>(key), static_cast<char>(key));

		KeyPress(key);

		if(!Focus()) return;

		if(id_ == Focus())
		{
			KeyLock(shared_from_this());
			i_doKeyDown(key);
		}
		else
		{
			for(auto& p : children_)
			{
				if(p->hasFocus())
				{
					p->keyDown(key);
					break;
				}
			}
		}
	}

	void Surface::keyUp(Controls key)
	{
		LOG("Key %u(%c) is released.", static_cast<DWORD>(key), static_cast<char>(key));

		if(Surface_ptr p = KeyLock())
		{
			p->i_doKeyUp(key);
		}

		KeyRelease(key);
	}

	void Surface::mouseDown(MouseButtons key, int x, int y)
	{
		LOG("Mouse(%u) is down @%d,%d", static_cast<DWORD>(key), x, y);

		x -= x1_;
		y -= y1_;

		if(mlock_) return;

		for(auto& p : children_)
		{
			if(    x >= p->x1_ && x < p->x2_
				&& y >= p->y1_ && y < p->y2_)
			{
				mlock_ = p;
				p->mouseDown(key, x, y);
				return;
			}
		}

		i_doMouseDown(key, x, y);
	}

	void Surface::mouseMove(int x, int y)
	{
		x -= x1_;
		y -= y1_;

		if(mlock_)
		{
			mlock_->mouseMove(x, y);
		}
		else
		{
			for(auto& p : children_)
			{
				if(    x >= p->x1_ && x < p->x2_
					&& y >= p->y1_ && y < p->y2_)
				{
					p->mouseMove(x, y);
					return;
				}
			}

			WFocus(id_);

			Surface_ptr p = MouseLock();

			if(p && p->id_ != id_)
			{
				p->i_doMouseOver(false);
			}

			if(!p || p->id_ != id_)
			{
				i_doMouseOver(true);
			}

			MouseLock(shared_from_this());

			i_doMouseMove(x, y);
		}
	}

	void Surface::mouseUp(MouseButtons key, int x, int y)
	{
		LOG("Mouse(%u) is up @%d,%d", static_cast<DWORD>(key), x, y);

		x -= x1_;
		y -= y1_;

		if(mlock_)
		{
			mlock_->mouseUp(key, x, y);
			mlock_.reset();
		}
		else
		{
			i_doMouseUp(key, x, y);
		}
	}

	void Surface::scroll(int d)
	{
		if(mlock_)
		{
			return;
		}

		if(id_ == WFocus())
		{
			i_doScroll(d);
		}
		else
		{
			for(auto& p : children_)
			{
				if(p->hasWFocus())
				{
					p->scroll(d);
					break;
				}
			}
		}
	}

	Surface_ptr Surface::KeyLock(Surface_ptr p)
	{
		static Surface_wptr lock;
		
		if(p)
		{
			lock = p;
			p.reset();
		}
		else
		{
			p = lock.lock();
			lock.reset();
		}

		return p;
	}

	Surface_ptr Surface::MouseLock(Surface_ptr p)
	{
		static Surface_wptr lock;

		if(p)
		{
			lock = p;
			p.reset();
		}
		else
		{
			p = lock.lock();
			lock.reset();
		}

		return p;
	}

	point Surface::getAbsCoords(void) const
	{
		point p(x1_, y1_);

		if(Surface_ptr pp = parent_.lock())
		{
			point q(pp->getAbsCoords());
			p.x += q.x;
			p.y += q.y;
		}

		return p;
	}
}}

