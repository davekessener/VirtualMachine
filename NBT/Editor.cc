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
	registerCommand("open", [this](std::vector<std::string> args)
		{
			std::string fn(args.at(1));
			nbt::TAG_Compound::ptr_t tag = nbt::readFile(fn);
			NBTNode *n = new NBTNode;
			n->load(tag);
			tv_->setRoot(Node_ptr(n));
		});
}

void Editor::init(void)
{
	CommandableScene::init();
	int w, h;
	display::Terminal::instance().getScreenSize(w, h);
	tv_.reset(new TreeView(0, 0, w, h - 2));
}

void Editor::suspend(void)
{
}

void Editor::wake(void)
{
}

void Editor::input(int in)
{
	tv_->input(in);
}

void Editor::update(int ms)
{
}

void Editor::refresh(void)
{
	tv_->render();
	CommandableScene::refresh();
}

void Editor::finalize(void)
{
}


