#ifndef NBTEDIT_NBTNODE_H
#define NBTEDIT_NBTNODE_H

#include <nbt/NBT.h>
#include "Node.h"

class NBTNode : public Node
{
	public:
		explicit NBTNode(nbt::NBT_ptr_t p = nbt::NBT_ptr_t()) : Node() { load(p); }
		void load(nbt::NBT_ptr_t);
	private:
		std::string i_doGetContent( ) const;
	private:
		nbt::NBT_ptr_t tag_;
};

#endif

