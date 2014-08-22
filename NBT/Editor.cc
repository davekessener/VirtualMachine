#include <memory>
#include <vector>
#include "Editor.h"
#include <nbt/NBT.h>
#include <dav/Logger.h>
#include <aux>
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
		void erase(bool = false);
		void insert(const params_t&);
		void rename(const params_t&);
		void set(const params_t&);
		void refresh( ) const;
		bool canQuit( ) const;
		void input(int);
	private:
		void checkTV( ) const;
		NBTNode& getNode( );
	private:
		std::shared_ptr<TreeView> tv_;
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
	Object_ptr o(new Object);
	o->load(args.at(1));
	tv_.reset(new TreeView(s_.x, s_.y, s_.w, s_.h - 1, o));
}

void Editor::Impl::write(const params_t& args)
{
	if(args.size() > 2) throw std::string("too many arguments! :save [filename]");
	if(!static_cast<bool>(tv_)) throw std::string("no savable buffer open!");

	std::string fn(args.size() == 2 ? args.at(1) : "");
	
	tv_->getObject()->save(fn);
	tv_->modify(false);
}

void Editor::Impl::close(bool force)
{
	if(force || canQuit())
	{
		tv_.reset();
	}
	else
	{
		throw std::string("modified buffer. write or force overwrite with '!'");
	}
}

bool Editor::Impl::canQuit(void) const
{
	return !static_cast<bool>(tv_) || !tv_->isModified();
}

void Editor::Impl::input(int in)
{
	if(tv_) tv_->input(in);
}

void Editor::Impl::erase(bool force)
{
	checkTV();

	NBTNode &node(getNode());
	
	if(!node.hasParent()) throw std::string("cannot delete root!");
	
	node.parent()->erase(force);
	tv_->modify();
}

void Editor::Impl::insert(const params_t& args)
{
	checkTV();
	if(args.size() / 2 != 1) throw std::string("invalid arguments! :insert ID [name]");

	std::string name(args.size() == 3 ? args.at(2) : "");
	int id = 0;

	using lib::aux::lexical_cast;

	try
	{
		if(!args.at(1).empty()) id = lexical_cast<int>(args.at(1));
	}
	catch(...)
	{
		char c(args.at(1).at(0));
		id = 0;
		
		if(args.at(1).size() == 1 && c >= 'a' && c <= 'b') id = c - 'a' + 10;
	}

	getNode().insert(id, name);
	tv_->modify();
}

void Editor::Impl::rename(const params_t& args)
{
	checkTV();
	if(args.size() != 2) throw std::string("need new name! :rename newname");

	getNode().rename(args.at(1));
	tv_->modify();
}

void Editor::Impl::set(const params_t& args)
{
	checkTV();
	if(args.size() != 2) throw std::string("bad form! :set \"value\"");

	getNode().set(args.at(1));
	tv_->modify();
}

void Editor::Impl::refresh(void) const
{
	if(tv_) tv_->render();
}

void Editor::Impl::checkTV(void) const
{
	if(!static_cast<bool>(tv_)) throw std::string("no editable buffer open!");
}

NBTNode& Editor::Impl::getNode(void)
{
	return *std::dynamic_pointer_cast<NBTNode>(tv_->getNode());
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
#define CMD(name) registerCommand(name, [this](const params_t& args, bool force)
#define CEND )
	CMD("quit")   { impl_->close(force); quit(); } CEND;
//	CMD("echo")   { for(const std::string& s : args) LOG("echoed '%s'", s.data()); } CEND;
	CMD("edit")   { impl_->close(force); impl_->edit(args); } CEND;
	CMD("write")  { impl_->write(args);  } CEND;
	CMD("close")  { impl_->close(force); } CEND;
	CMD("delete") { impl_->erase(force); } CEND;
	CMD("insert") { impl_->insert(args); } CEND;
	CMD("rename") { impl_->rename(args); } CEND;
	CMD("set")    { impl_->set(args);    } CEND;
#undef CEND
#undef CMD
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


