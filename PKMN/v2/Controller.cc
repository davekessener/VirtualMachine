#include "Controller.h"
#include <boost/filesystem.hpp>
#include "Map.h"
#include "Config.h"
#include "Player.h"
#include "Camera.h"
#include "Keypad.h"

std::shared_ptr<pkmn::Controller> pkmn::Controller::controller;

namespace pkmn
{
	struct Controller::Impl
	{
		private:
			Impl(nbt::TAG_Compound::ptr_t);
			~Impl( );
			void update(int);
			void render( ) const;
		private:
			friend class Controller;
			Player player_;
			Camera camera_;
			std::shared_ptr<Map> map_;
	};

// # ===========================================================================

	bool Controller::load(const std::string& p)
	{
		if(!boost::filesystem::is_regular_file(p)) return false;

		nbt::TAG_Compound::ptr_t tag = nbt::readFile(p);

		if(!static_cast<bool>(tag)) throw std::string("file '" + p + "' could not be read.");

		controller.reset(new Controller(tag));

		return true;
	}

	void Controller::generate(const std::string& name)
	{
		controller.reset(new Controller(Player::generate(name)));
	}

	Controller& Controller::instance(void)
	{
		if(!static_cast<bool>(controller)) throw std::string("no save file loaded");

		return *controller;
	}

	Controller::Controller(nbt::TAG_Compound::ptr_t p) : impl_(new Impl(p))
	{
	}

	Controller::~Controller(void)
	{
		nbt::writeFile("save.nbt", impl_->player_.save());

		delete impl_;
	}

	void Controller::update(int d)
	{
		impl_->update(d);
	}

	void Controller::render(void) const
	{
		impl_->render();
	}

// # ===========================================================================

	void Controller::Impl::update(int d)
	{
		player_.update(d);

		switch(Key key = Keypad::recent())
		{
			case Key::UP:
			case Key::DOWN:
			case Key::LEFT:
			case Key::RIGHT:
				player_.move(static_cast<Direction>(static_cast<DWORD>(key)));
				break;
			case Key::NONE:
				break;
			default:
				break;
		}
	}

	void Controller::Impl::render(void) const
	{
		point o(camera_.getOffset());
		o.x = Config::SCREEN_WIDTH / 2 - o.x;
		o.y = Config::SCREEN_HEIGHT / 2 - o.y;

		map_->render(o.x, o.y);
		player_.render(o.x, o.y);
	}

	Controller::Impl::Impl(nbt::TAG_Compound::ptr_t p) : player_(p), camera_()
	{
		camera_.lock(player_);

		map_ = std::shared_ptr<Map>(new Map(player_.map()));
	}

	Controller::Impl::~Impl(void)
	{
	}
}

