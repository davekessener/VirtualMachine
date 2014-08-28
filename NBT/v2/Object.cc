#include "Object.h"
#include "NBTNode.h"
#include "FileSystem.h"

Object::Object(void)
{
	NBTNode *nbt = new NBTNode;
	node_.reset(nbt);

	nbt->load(nbt::Make<nbt::TAG_Compound>());
}

Object::~Object(void)
{
	deleteBackup();
}

void Object::load(const std::string& fn)
{
	nbt::NBT_ptr_t tag = nbt::readFile(fn);
	
	if(!tag) throw std::string("couldn't read file!");

	bck_ = backup(fn_ = fn);

	NBTNode *nbt = new NBTNode;

	node_.reset(nbt);

	nbt->load(tag);
}

void Object::save(const std::string& fn)
{
	if(!fn.empty()) fn_ = fn;
	if(fn_.empty()) throw std::string("no filename for buffer!");

	NBTNode &node(*dynamic_cast<NBTNode *>(&*node_));

	nbt::writeFile(fn_, std::dynamic_pointer_cast<nbt::TAG_Compound>(node.getTag()));
}

void Object::restore(void)
{
	if(bck_.empty()) throw std::string("no backup to restore from!");
	FileSystem::copyFile(bck_, fn_);
	load(fn_);
}

void Object::deleteBackup(void)
{
	if(!bck_.empty())
	{
		FileSystem::removeFile(bck_);
		bck_ = "";
	}
}

std::string Object::backup(const std::string& path) const
{
	std::string fn(FileSystem::getPath(path) + "." + FileSystem::getFile(path) + ".bak");
	FileSystem::copyFile(path, fn);
	return fn;
}

