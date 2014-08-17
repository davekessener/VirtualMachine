#include "NBTNode.h"

void NBTNode::load(nbt::NBT_ptr_t tag)
{
	tag_ = tag;

	Node::vec_t &vec(getChildren());

	Node::vec_t().swap(vec);

	switch(tag->getID())
	{
		case nbt::TAG_List::ID:
		{
			nbt::TAG_List::ptr_t p(std::dynamic_pointer_cast<nbt::TAG_List>(tag));
			for(const auto& t : *p) vec.push_back(Node_ptr(new NBTNode(t)));
			break;
		}
		case nbt::TAG_Compound::ID:
		{
			nbt::TAG_Compound::ptr_t p(std::dynamic_pointer_cast<nbt::TAG_Compound>(tag));
			for(const auto& t : p->Tags) vec.push_back(Node_ptr(new NBTNode(t)));
			break;
		}
	}
}

std::string NBTNode::i_doGetContent(void) const
{
	if(!static_cast<bool>(tag_)) return std::string();

	std::string s;

	return s;
}

