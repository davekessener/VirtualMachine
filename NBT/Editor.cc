#include "Editor.h"
#include <nbt/NBT.h>
#include "Terminal.h"
#include "NBTNode.h"

Editor::Editor(void)
{
}

Editor::~Editor(void)
{
}

void Editor::registerCommands(void)
{
	registerCommand("quit", std::bind(&Editor::quit, this));
	registerCommand("edit", [this](std::vector<std::string> args)
		{
			if(args.size() != 2) throw std::string("needs only one argument! :edit filename");
			Object_ptr o(new Object);
			o->load(args.at(1));
			tv_.reset(new TreeView(winsize_.x, winsize_.y, winsize_.w, winsize_.h - 2, o));
			o_ = o;
		});
	registerCommand("write", [this](std::vector<std::string> args)
		{
			if(args.size() > 2) throw std::string("too many arguments! :save [filename]");
			std::string fn(args.size() == 2 ? args.at(1) : "");
			o_->save(fn);
		});
}

void Editor::init(void)
{
	CommandableScene::init();

	winsize_.x = winsize_.y = 0;
	display::Terminal::instance().getScreenSize(winsize_.w, winsize_.h);
}

void Editor::suspend(void)
{
}

void Editor::wake(void)
{
}

void Editor::input(int in)
{
	if(tv_) tv_->input(in);
}

void Editor::update(int ms)
{
}

void Editor::refresh(void)
{
	if(tv_) tv_->render();
	CommandableScene::refresh();
}

void Editor::finalize(void)
{
}


