#include <memory>
#include <vector>
#include "Editor.h"
#include <nbt/NBT.h>
#include "Terminal.h"
#include "NBTNode.h"
#include "TreeView.h"
#include "Object.h"
#include "Control.h"

struct Editor::Impl
{
	public:
		void init( );
		void edit(const params_t&);
		void write(const params_t&);
		void close(bool = false);
		bool canQuit( ) const;
		void input(int);
		void refresh( ) const;
	private:
		std::shared_ptr<TreeView> tv_;
		std::vector<Control_ptr> controls_;
		Object_ptr obj_;
		struct { int x, y, w, h; } s_;
};

void Editor::Impl::init(void)
{
	s_.x = s_.y = 0;
	display::Terminal::instance().getScreenSize(s_.w, s_.h);
}

void Editor::Impl::edit(const params_t& args)
{
	if(args.size() != 2) throw std::string("needs only one argument! :edit filename");
	obj_.reset(new Object);
	obj_->load(args.at(1));
	controls_.clear();
	tv_.reset(new TreeView(s_.x, s_.y, s_.w, s_.h - 1, obj_));
}

void Editor::Impl::write(const params_t& args)
{
	if(args.size() > 2) throw std::string("too many arguments! :save [filename]");
	if(!static_cast<bool>(obj_)) throw std::string("no savable buffer open!");

	std::string fn(args.size() == 2 ? args.at(1) : "");
	obj_->save(fn);
	tv_->modify(false);
}

void Editor::Impl::close(bool force)
{
	if(force || canQuit())
	{
		obj_.reset();
		tv_.reset();
		controls_.clear();
	}
	else
	{
		throw std::string("modified buffer. write or force overwrite with '!'");
	}
}

bool Editor::Impl::canQuit(void) const
{
	return static_cast<bool>(tv_) ? !tv_->isModified() : true;
}

void Editor::Impl::input(int in)
{
	if(!controls_.empty()) controls_.back()->input(in);
	else if(tv_) tv_->input(in);
}

void Editor::Impl::refresh(void) const
{
	if(tv_) tv_->render();
	for(const auto& p : controls_) p->render();
}

// # ===========================================================================

Editor::Editor(void) : impl_(new Impl)
{
}

Editor::~Editor(void)
{
	delete impl_;
}

void Editor::registerCommands(void)
{
	registerCommand("quit", [this](const params_t& args, bool force)
		{
			impl_->close(force);
			quit();
		});
	registerCommand("edit", [this](const params_t& args, bool force)
		{
			impl_->close(force);
			impl_->edit(args);
		});
	registerCommand("write", [this](const params_t& args, bool force)
		{
			impl_->write(args);
		});
	registerCommand("close", [this](const params_t& args, bool force)
		{
			impl_->close(force);
		});
}

void Editor::init(void)
{
	CommandableScene::init();
	impl_->init();
}

void Editor::suspend(void)
{
}

void Editor::wake(void)
{
}

void Editor::input(int in)
{
	impl_->input(in);
}

void Editor::update(int ms)
{
}

void Editor::refresh(void)
{
	impl_->refresh();
	CommandableScene::refresh();
}

void Editor::finalize(void)
{
}


