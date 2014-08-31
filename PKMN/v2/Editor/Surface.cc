#include "Surface.h"
#include <map>

namespace editor
{
	void Surface::keyDown(Controls key)
	{
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
		KeyLock()->i_doKeyUp(key);
	}

	void Surface::mouseDown(Controls key, int x, int y)
	{
		for(auto& p : children_)
		{
			if(    x >= p->x1_ && x < p->x2_
				&& y >= p->y1_ && y < p->y2_)
			{
				mlock_ = p;
				p->mouseDown(key, x - x1_, y - y1_);
				return;
			}
		}

		i_doMouseDown(key, x, y);
	}

	void Surface::mouseMove(int x, int y)
	{
		if(mlock_)
		{
			mlock_->mouseMove(x - x1_, y - y1_);
		}
		else
		{
			for(auto& p : children_)
			{
				if(    x >= p->x1_ && x < p->x2_
					&& y >= p->y1_ && y < p->y2_)
				{
					p->mouseMove(x - x1_, y - y1_);
					return;
				}
			}

			i_doMouseMove(x, y);
		}
	}

	void Surface::mouseUp(Controls key, int x, int y)
	{
		if(mlock_)
		{
			mlock_->mouseUp(key, x - x1_, y - y1_);
			mlock_.reset();
		}
		else
		{
			i_doMouseDown(key, x, y);
		}
	}

	Surface_ptr Surface::KeyLock(Surface_ptr p)
	{
		static Surface_ptr lock;
		
		if(p)
		{
			lock = p;
			p.reset();
		}
		else
		{
			p = lock;
			lock.reset();
		}

		return p;
	}
}

