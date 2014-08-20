#include "Object.h"
#include "NBTNode.h"

Object::Object(void)
{
	NBTNode *nbt = new NBTNode;
	node_.reset(nbt);

	nbt->load(nbt::Make<nbt::TAG_Compound>());
}

void Object::load(const std::string& fn)
{
	fn_ = fn;

	nbt::NBT_ptr_t tag = nbt::readFile(fn_);
	
	if(!tag) throw std::string("couldn't read file!");

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

