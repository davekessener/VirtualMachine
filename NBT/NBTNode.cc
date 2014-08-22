#include <sstream>
#include <cassert>
#include <aux>
#include "NBTNode.h"
#include <dav/Logger.h>

#define MXT_MAXARRAY 16

namespace
{
	template<BYTE ID, typename T>
		void printTag(std::shared_ptr<nbt::NBTSimple<ID, T>>, std::stringstream&);
	template<BYTE ID, typename T1, typename T2>
		void printTag(std::shared_ptr<nbt::NBTArray<ID, T1, T2>>, std::stringstream&);
	void printTag(nbt::TAG_String::ptr_t, std::stringstream&);
	void printTag(nbt::TAG_List::ptr_t, std::stringstream&);
	void printTag(nbt::TAG_Compound::ptr_t, std::stringstream&);

	enum
	{
		NOERR = 0,
		ERR_NOERASE
	};
}

void NBTNode::load(nbt::NBT_ptr_t tag)
{
	tag_ = tag;

	Node::vec_t &vec(getChildren());

	Node::vec_t().swap(vec);

	LOG("Loaded node with tag-ID %02x", tag->getID());

	auto generate = [this](nbt::NBT_ptr_t nbt) -> Node_ptr
		{
			NBTNode *n(new NBTNode);
			Node_ptr p(n); // <-- IMPORTANT!!!

			n->load(nbt);
			n->setParent(shared_from_this());

			return p;
		};

	switch(tag->getID())
	{
		case nbt::TAG_List::ID:
		{
			nbt::TAG_List::ptr_t p(std::dynamic_pointer_cast<nbt::TAG_List>(tag));
			for(const auto& t : *p) vec.push_back(generate(t));
			break;
		}
		case nbt::TAG_Compound::ID:
		{
			nbt::TAG_Compound::ptr_t p(std::dynamic_pointer_cast<nbt::TAG_Compound>(tag));
			for(const auto& t : p->Tags) vec.push_back(generate(t));
			break;
		}
	}

	dirty();
}

void NBTNode::insert(BYTE id, const std::string& name)
{
	if(id < 1 || id > 11) throw std::string("invalid id!");

	nbt::NBT_ptr_t tag = nbt::Make(id, name);

	switch(tag_->getID())
	{
		case nbt::TAG_List::ID:
		{
			nbt::TAG_List::ptr_t list = std::dynamic_pointer_cast<nbt::TAG_List>(tag_);
			if(list->tagID() && list->tagID() != id) throw std::string("invalid type for list!");
			list->addTag(tag);
			break;
		}
		case nbt::TAG_Compound::ID:
		{
			nbt::TAG_Compound::ptr_t nbt = std::dynamic_pointer_cast<nbt::TAG_Compound>(tag_);
			nbt->setTag(tag);
			break;
		}
		default:
			throw std::string("only lists and compounds can have tags!");
	}

	NBTNode *node = new NBTNode;
	Node_ptr pp(node);

	node->load(tag);
	
	addChild(pp);
	dirty();
}

void NBTNode::rename(const std::string& name)
{
	if(!hasParent())
	{
		throw std::string("cannot name root!");
	}

	nbt::NBT_ptr_t tag = std::dynamic_pointer_cast<NBTNode>(current())->tag_;
	nbt::NBT_ptr_t parent = tag_;

	if(tag->getName() == name) throw std::string("tag already has that name!");

	if(parent->getID() == nbt::TAG_List::ID) throw std::string("list elements cannot be named!");

	assert(parent->getID()==nbt::TAG_Compound::ID);

	nbt::TAG_Compound::ptr_t nbt = std::dynamic_pointer_cast<nbt::TAG_Compound>(parent);

	if(nbt->hasTag(name)) throw std::string("an element with that name already exists!");

	nbt->removeTag(tag->getName());
	nbt->setTag(name, tag);
	
	dirty();
}

std::string NBTNode::i_doGetContent(void) const
{
	LOG("Generating content for tag %02x", tag_->getID());

	if(!static_cast<bool>(tag_)) return std::string();

	std::stringstream ss;

	ss << "[" << lib::aux::to_hex(tag_->getID())[1] << "] ";
	
	if(!tag_->getName().empty()) ss << "'" << tag_->getName() << "': ";

	switch(tag_->getID())
	{
		case nbt::TAG_Byte::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Byte>(tag_), ss);
			break;
		case nbt::TAG_Short::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Short>(tag_), ss);
			break;
		case nbt::TAG_Int::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Int>(tag_), ss);
			break;
		case nbt::TAG_Long::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Long>(tag_), ss);
			break;
		case nbt::TAG_Float::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Float>(tag_), ss);
			break;
		case nbt::TAG_Double::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Double>(tag_), ss);
			break;
		case nbt::TAG_String::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_String>(tag_), ss);
			break;
		case nbt::TAG_Byte_Array::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Byte_Array>(tag_), ss);
			break;
		case nbt::TAG_List::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_List>(tag_), ss);
			break;
		case nbt::TAG_Compound::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Compound>(tag_), ss);
			break;
		case nbt::TAG_Int_Array::ID:
			printTag(std::dynamic_pointer_cast<nbt::TAG_Int_Array>(tag_), ss);
			break;
	}

	return ss.str();
}

void NBTNode::i_doProcessError(uint err) const
{
	switch(err)
	{
		case ERR_NOERASE:
			throw std::string("node has children. if you are sure, use '!' to override.");
	}
}

NBTNode::uint NBTNode::i_canErase(Node_ptr p) const
{
	return p->hasChildren() ? ERR_NOERASE : NOERR;
}

void NBTNode::i_doErase(Node_ptr p)
{
	NBTNode &node(*dynamic_cast<NBTNode *>(&*p));

	switch(tag_->getID())
	{
		case nbt::TAG_List::ID:
		{
			nbt::TAG_List::ptr_t list = std::dynamic_pointer_cast<nbt::TAG_List>(tag_);
			list->removeTag(node.tag_);
			break;
		}
		case nbt::TAG_Compound::ID:
		{
			nbt::TAG_Compound::ptr_t nbttagcompound = std::dynamic_pointer_cast<nbt::TAG_Compound>(tag_);
			nbttagcompound->removeTag(node.tag_->getName());
			break;
		}
	}
}

namespace
{
	template<typename T> struct traits { typedef long display_type; };
	template<> struct traits<float> { typedef double display_type; };
	template<> struct traits<double> { typedef double display_type; };

	template<BYTE ID, typename T>
	void printTag(std::shared_ptr<nbt::NBTSimple<ID, T>> tag, std::stringstream& ss)
	{
		ss << static_cast<typename traits<T>::display_type>(tag->get());
	}

	template<BYTE ID, typename T1, typename T2>
	void printTag(std::shared_ptr<nbt::NBTArray<ID, T1, T2>> tag, std::stringstream& ss)
	{
		ss << "{";
		
		int c(0);
		for(const auto& v : *tag)
		{
			if(c) ss << ", ";
			if(++c > MXT_MAXARRAY)
			{
				ss << "[...]";
				break;
			}
			ss << v << "(0x" << lib::aux::to_hex(v) << ")";
		}

		ss << "}";
	}

	void printTag(nbt::TAG_String::ptr_t tag, std::stringstream& ss)
	{
		ss << "'" << tag->get() << "'";
	}

	void printTag(nbt::TAG_List::ptr_t tag, std::stringstream& ss)
	{
		ss << "Contains " << tag->size() << " elements of type " << tag->tagID();
	}

	void printTag(nbt::TAG_Compound::ptr_t tag, std::stringstream& ss)
	{
		ss << "Contains " << tag->size() << " tags.";
	}
}

