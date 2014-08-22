#ifndef NBTEDIT_NBTNODE_H
#define NBTEDIT_NBTNODE_H

#include <nbt/NBT.h>
#include "Node.h"

class NBTNode : public Node
{
	using Node::uint;

	public:
		explicit NBTNode(nbt::NBT_ptr_t p = nbt::NBT_ptr_t()) : Node() { if(static_cast<bool>(p)) load(p); }
		void load(nbt::NBT_ptr_t);
		nbt::NBT_ptr_t getTag( ) { return tag_; }
		void insert(BYTE, const std::string&);
		void rename(const std::string& s);
	private:
		std::string i_doGetContent( ) const;
		void i_doProcessError(uint) const;
		uint i_canErase(Node_ptr) const;
		void i_doErase(Node_ptr);
	private:
		nbt::NBT_ptr_t tag_;
};

#endif

