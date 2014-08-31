#include "Keypad.h"
#include "Config.h"

namespace pkmn
{
	Keypad::Keypad(void)
	{
		auto reg = [this](Key k, Controls c) { lookup_[k] = c; rlook_[c] = k; };

		reg(Key::UP, static_cast<Controls>(Config::KEY_UP));
		reg(Key::DOWN, static_cast<Controls>(Config::KEY_DOWN));
		reg(Key::LEFT, static_cast<Controls>(Config::KEY_LEFT));
		reg(Key::RIGHT, static_cast<Controls>(Config::KEY_RIGHT));
		reg(Key::A, static_cast<Controls>(Config::KEY_A));
		reg(Key::B, static_cast<Controls>(Config::KEY_B));
		reg(Key::L, static_cast<Controls>(Config::KEY_L));
		reg(Key::R, static_cast<Controls>(Config::KEY_R));
		reg(Key::START, static_cast<Controls>(Config::KEY_START));
		reg(Key::SELECT, static_cast<Controls>(Config::KEY_SELECT));
	}

	bool Keypad::doIsPressed(Key key) const
	{
		return keys_.find(lookup_.at(key)) != keys_.cend();
	}

	void Keypad::doPress(Controls key, bool pressed)
	{
		if(rlook_.find(key) == rlook_.end()) return;

		auto i = keys_.find(key);

		if(pressed)
		{
			if(i == keys_.end())
			{
				keys_.insert(key);
				history_.push_back(rlook_.at(key));
			}
		}
		else
		{
			if(i != keys_.end())
			{
				keys_.erase(i);
				history_.erase(std::find(history_.begin(), history_.end(), rlook_.at(key)));
			}
		}
	}

	Key Keypad::doRecent(void) const
	{
		return keys_.empty() ? Key::NONE : history_.back();
	}
}

