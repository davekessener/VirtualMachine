#include <sstream>
#include <cassert>
#include <aux>
#include "NBTNode.h"
#include <dav/Logger.h>
#include <dav/utils.h>
#include <aux>

#define MXT_MAXARRAY 8

using dav::aux::lexical_cast;

namespace
{
	template<BYTE ID, typename T>
		void printTag(std::shared_ptr<nbt::NBTSimple<ID, T>>, std::stringstream&);
	template<BYTE ID, typename T1, typename T2>
		void printTag(std::shared_ptr<nbt::NBTArray<ID, T1, T2>>, std::stringstream&);
	void printTag(nbt::TAG_String::ptr_t, std::stringstream&);
	void printTag(nbt::TAG_List::ptr_t, std::stringstream&);
	void printTag(nbt::TAG_Compound::ptr_t, std::stringstream&);
	template<typename T, BYTE id>
		void readArray(std::shared_ptr<nbt::NBTArray<id, DWORD, T>>, const std::string&);

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

//	LOG("Loaded node with tag-ID %02x", tag->getID());

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
			if(name.empty()) throw std::string("you can only add nameless tags to lists!");
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

	nbt::NBT_ptr_t parent_tag = std::dynamic_pointer_cast<NBTNode>(parent())->tag_;

	if(tag_->getName() == name) throw std::string("tag already has that name!");

	if(parent_tag->getID() == nbt::TAG_List::ID) throw std::string("list elements cannot be named!");

	assert(parent_tag->getID()==nbt::TAG_Compound::ID);

	nbt::TAG_Compound::ptr_t nbt = std::dynamic_pointer_cast<nbt::TAG_Compound>(parent_tag);

	if(nbt->hasTag(name)) throw std::string("an element with that name already exists!");

	nbt->removeTag(tag_->getName());
	nbt->setTag(name, tag_);
	
	dirty();
}

void NBTNode::set(const std::string& content)
{
	switch(tag_->getID())
	{
		case nbt::TAG_Byte::ID:
		{
			long v = dav::utils::toInt(content);
			if(v != static_cast<char>(v)) throw std::string("number exceeds Byte boundaries!");
			std::dynamic_pointer_cast<nbt::TAG_Byte>(tag_)->set(v);
			break;
		}
		case nbt::TAG_Short::ID:
		{
			long v = dav::utils::toInt(content);
			if(v != static_cast<short>(v)) throw std::string("number exceeds Short boundaries!");
			std::dynamic_pointer_cast<nbt::TAG_Short>(tag_)->set(v);
			break;
		}
		case nbt::TAG_Int::ID:
		{
			long v = dav::utils::toInt(content);
			if(v != static_cast<int>(v)) throw std::string("number exceeds Int boundaries!");
			std::dynamic_pointer_cast<nbt::TAG_Int>(tag_)->set(v);
			break;
		}
		case nbt::TAG_Long::ID:
		{
			long v = dav::utils::toInt(content);
			std::dynamic_pointer_cast<nbt::TAG_Long>(tag_)->set(v);
			break;
		}
		case nbt::TAG_Float::ID:
			std::dynamic_pointer_cast<nbt::TAG_Float>(tag_)->set(lexical_cast<float>(content));
			break;
		case nbt::TAG_Double::ID:
			std::dynamic_pointer_cast<nbt::TAG_Double>(tag_)->set(lexical_cast<double>(content));
			break;
		case nbt::TAG_String::ID:
			std::dynamic_pointer_cast<nbt::TAG_String>(tag_)->set(content);
			break;
		case nbt::TAG_Byte_Array::ID:
			readArray<BYTE>(std::dynamic_pointer_cast<nbt::TAG_Byte_Array>(tag_), content);
			break;
		case nbt::TAG_List::ID:
			throw std::string("cannot set content of TagList!");
		case nbt::TAG_Compound::ID:
			throw std::string("cannot set content of CompoundTag!");
		case nbt::TAG_Int_Array::ID:
			readArray<DWORD>(std::dynamic_pointer_cast<nbt::TAG_Int_Array>(tag_), content);
			break;
	}

	dirty();
}

std::string NBTNode::i_doGetContent(void) const
{
//	LOG("Generating content for tag %02x", tag_->getID());

	if(!static_cast<bool>(tag_)) return std::string();

	std::stringstream ss;

	ss << "[" << dav::aux::to_hex(tag_->getID())[1] << "] ";
	
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

	template<typename T> struct sign_traits { typedef T unsigned_type; typedef T signed_type; };
	template<> struct sign_traits<BYTE> { typedef BYTE unsigned_type; typedef char signed_type; };
	template<> struct sign_traits<WORD> { typedef WORD unsigned_type; typedef short signed_type; };
	template<> struct sign_traits<DWORD> { typedef DWORD unsigned_type; typedef int signed_type; };
	template<> struct sign_traits<QWORD> { typedef QWORD unsigned_type; typedef long signed_type; };

	template<BYTE ID, typename T>
	void printTag(std::shared_ptr<nbt::NBTSimple<ID, T>> tag, std::stringstream& ss)
	{
		ss << static_cast<typename traits<T>::display_type>(static_cast<typename sign_traits<T>::signed_type>(tag->get()));
		ss << "(0x" << dav::aux::to_hex(tag->get(), sizeof(T)) << ")";
	}

	template<BYTE ID, typename T1, typename T2>
	void printTag(std::shared_ptr<nbt::NBTArray<ID, T1, T2>> tag, std::stringstream& ss)
	{
		ss << "[" << (int)tag->size() << "]{";
		
		int c(0);
		for(const auto& v : *tag)
		{
			if(c) ss << ", ";
			if(++c > MXT_MAXARRAY)
			{
				ss << "[...]";
				break;
			}
			ss	<< static_cast<typename traits<T2>::display_type>(v)
				<< "(0x" << dav::aux::to_hex(v) << ")";
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

	template<typename T, BYTE id>
		void readArray(std::shared_ptr<nbt::NBTArray<id, DWORD, T>> tag, const std::string& content)
	{
		typedef typename sign_traits<T>::unsigned_type unsigned_t;
		typedef typename sign_traits<T>::signed_type signed_t;

		using dav::utils::next;
		using dav::utils::toInt;

		LOG("Reading array '%s'", content.data());

		std::string left(content);
		std::string s(next(left)), old;
//		bool skip = true;
		const long min = static_cast<signed_t>(1 << (sizeof(T) * 8 - 1));
		const unsigned long max = static_cast<unsigned_t>(-1);
		size_t i = 0;
		bool overwrite = false, isend = false;

		LOG("Bound by [%li, %lu]", min, max);

		LOG("Token '%s'", s.data());

		if(s == "^")
		{
			LOG("Modify existing");
			s = next(left);
			LOG("Token '%s'", s.data());
			i = tag->size();
		}
		else
		{
			LOG("Clearing array");
			tag->clear();
		}

		if(s != "{") throw std::string("malformed array! [^]{val/op, ... }");

		while(!left.empty())
		{
			if(isend) throw std::string("malformed array! exceeds end");

			old.clear();
			s = next(left);

			while(s != "," && s != "}")
			{
				old.append(s);
				s = next(left);
				if(s != "}" && left.empty()) throw std::string("malformed array! premature end");
			}

			LOG("Token '%s' + '%s'", old.data(), s.data());

			if(s == "}") isend = true;

			s = next(old);
			if(s == "/")
			{
				if(old.empty()) throw std::string("malformed array op! incomplete");
				s = next(old);
				if(s == "@")
				{
					if(old.empty()) throw std::string("empty address");
					long a = toInt(next(old));
					if(a < 0 || a > (long)tag->size())
					{
						LOG("ERR: Invalid index %ld/%lu", a, tag->size());
						throw std::string("invalid index");
					}
					i = a;
					LOG("# Moved to %d", i);
				}
				else if(s == "+")
				{
					if(old.empty()) throw std::string("empty offset");
					long a = toInt(next(old));
					if(a < 0 || a + (long)i > (long)tag->size())
					{
						LOG("ERR: Invalid offset @%i+%ld/%lu", i, a, tag->size());
						throw std::string("invalid offset");
					}
					i += a;
					LOG("# Jumped by %ld to %d", a, i);
				}
				else if(s == "-")
				{
					if(old.empty()) throw std::string("empty offset");
					long a = toInt(next(old));
					if(a < 0 || (long)i - a < 0)
					{
						LOG("ERR: Invalid offset @%i-%ld/%lu", i, a, tag->size());
						throw std::string("invalid offset");
					}
					i -= a;
					LOG("# Jumped by -%ld to %d", a, i);
				}
				else if(s == "$")
				{
					i = tag->size();
					LOG("# Jumped to end (%d)", i);
				}
				else if(s == "^")
				{
					i = 0;
					LOG("# Jumped to beginning");
				}
				else if(s == "#")
				{
					overwrite = !overwrite;
					LOG("# %s overwrite mode", overwrite ? "Enabled" : "Disabled");
				}
				else if(s == "!")
				{
					int o = i, l = 1;
					if(!old.empty())
					{
						o = toInt(s = next(old));
						if(!old.empty())
						{
							if((s = next(old)) == "-")
							{
								l = toInt(old) - o;
							}
							else if(s == "+")
							{
								l = toInt(old);
							}
							else
							{
								LOG("ERR: malformed delete, expected '-' ('%s')", s.data());
								throw std::string("malformed array op! expected '-'");
							}
						}
					}

					if(o < 0 || l <= 0 || o + l >= (long)tag->size())
					{
						LOG("ERR: Invalid delete: from %d with %d /%lu", o, l, tag->size());
						throw std::string("malformed array op! invalid delete range");
					}

					LOG("# Delete %d from %d", l, o);
					tag->erase(tag->begin() + o, tag->begin() + o + l);
					i = o;
				}
				else
				{
					LOG("ERR: unknown op '%s'", s.data());
					throw std::string("malformed array op! unknown op");
				}
			}
			else
			{
				long v = toInt(s + old);
				if(v < min || (unsigned long)v > max)
				{
					LOG("ERR: %ld exceeds boundaries!", v);
					throw std::string("value exceeds boundaries!");
				}
				if(!overwrite)
				{
					tag->insert(tag->begin() + i, v);
				}
				else
				{
					if(i < tag->size()) tag->at(i) = v;
					else tag->push_back(v);
				}
				LOG("### %s %ld at %i", overwrite ? "Wrote" : "Inserted", v, i);
				++i;
			}
		}

		if(!isend) throw std::string("malformed array! missing end");
//
//		while(!left.empty())
//		{
//			old = s;
//			s = next(left);
//
//			LOG("Token '%s'", s.data());
//
//			if(s == ",")
//			{
//				if(skip) throw std::string("malformed array!");
//				skip = true;
//				continue;
//			}
//			else if(s == "-")
//			{
//				skip = true;
//				continue;
//			}
//			else if(s == "}")
//			{
//				break;
//			}
//
//			if(old == "-")
//			{
//				s = old + s;
//			}
//
//			if(!skip) throw std::string("malformed array!");
//
//			skip = false;
//			long v = toInt(s);
//
//			if(v < min || (unsigned long)v > max) throw std::string("value exceeds boundaries!");
//
//			tag->push_back(v);
//		}
//
//		if(skip || s != "}") throw std::string("malformed array!");
	}
}

