#include "JSON.h"
#undef NDEBUG
#include <cassert>
#include <sstream>

namespace nbt { namespace json {

class JSON
{
	public:
		JSON(std::ostream& os) : os_(&os), is_(nullptr), ws_(0), look_(0) { }
		JSON(std::istream& is) : os_(nullptr), is_(&is), ws_(0), look_(0) { next(); }
#define DEF_WRITE(t) void write(t&);
		void write(NBTBase&, bool = true);
		DEF_WRITE(TAG_Byte);
		DEF_WRITE(TAG_Short);
		DEF_WRITE(TAG_Int);
		DEF_WRITE(TAG_Long);
		DEF_WRITE(TAG_Float);
		DEF_WRITE(TAG_Double);
		DEF_WRITE(TAG_Byte_Array);
		DEF_WRITE(TAG_String);
		DEF_WRITE(TAG_List);
		void write(TAG_Compound&, bool = true);
		DEF_WRITE(TAG_Int_Array);
#undef DEF_WRITE
#define DEF_READ(v,t) TAG_##v##_ptr_t read##t( );
		DEF_READ(Byte, Boolean);
		DEF_READ(Long, Number);
		DEF_READ(String, String);
		DEF_READ(Compound, Object);
		DEF_READ(List, Array);
#undef DEF_READ
	private:
		NBT_ptr_t readAny( );
		std::string readName( ) { std::string n(readRawString()); assertNext(':'); return n; }
		std::string readRawString( )
		{
			assertNext('"');
			std::vector<char> s;
			while(look_ != '"')
			{
				s.push_back(look_);
				look_ = is_->get();
			}
			next();
			return std::string(s.begin(), s.end());
		}
		template<typename T> T readRawNumber( )
		{
			long sign(look_ == '-' ? -1 : 1);

			if(look_ != '-') is_->unget();

			std::string s(""); long v;
			while((look_ = is_->get()) >= '0' && look_ <= '9') s.push_back((char)look_);
			std::stringstream ss(s); ss >> v;

			return v * sign;
		}
		void next( ) { while((look_ = is_->get()) == ' ' || look_ == '\t' || look_ == '\n'); std::cerr << "NEW: '" << (char)look_ << "'" << std::endl; }
		void assertNext(int v) { if(v != look_) throw std::string("ERR: Expected " + (char)v); next(); }
	private:
		void newLine( )
			{ if(ws_ >= 0) { (*os_) << '\n'; for(int i(ws_) ; i ; --i) (*os_) << '\t'; } }
		void writeName(const std::string& s)
			{ (*os_) << '"' << s << "\" : "; }
		template<typename T> void writeNumber(BYTE id, const T& v)
			{ (*os_) << "{ \"type\" : " << id << ", \"value\" : " << v << " }"; }
		template<typename T> void writeArray(const T& a)
		{
			bool f = true;
			(*os_) << "{ \"type\" : " << T::ID << ", "
				<< "\"value\" : [ ";
			
			for(const auto& v : a)
			{
				if(!f) (*os_) << ", ";
				(*os_) << v;
			}

			(*os_) << " }";
		}
	private:
		std::ostream *os_;
		std::istream *is_;
		int ws_, look_;
};

// # ===========================================================================

TAG_Compound_ptr_t read(std::istream& is)
{
	JSON reader(is);

	return reader.readObject();
}

void write(std::ostream& os, TAG_Compound_ptr_t nbt)
{
	JSON writer(os);

	writer.write(*nbt, false);

	os << '\n' << std::endl;
}

// # ===========================================================================

TAG_Byte_ptr_t JSON::readBoolean(void)
{
	std::string s("");
	bool v;

	while(look_ >= 'a' && look_ <= 'z')
	{
		s.push_back(look_);
		next();
	}

	if(s == "true") v = true;
	else if(s == "false") v = false;
	else throw std::string("ERR: Excpected boolean '" + s + "'!");

	return Make<TAG_Byte>("", v);
}

TAG_Long_ptr_t JSON::readNumber(void)
{
	return Make<TAG_Long>("", readRawNumber<long>());
}

TAG_String_ptr_t JSON::readString(void)
{
	return Make<TAG_String>("", readRawString());
}

NBT_ptr_t JSON::readAny(void)
{
	NBT_ptr_t p;

	std::cerr << "READING ANYTHING!" << std::endl << "LOOKAHEAD IS " << look_ << std::flush << " '" << (char)look_ << "'" << std::endl;

	if(look_ == '-' || (look_ >= '0' && look_ <= '9'))
	{
		p = std::dynamic_pointer_cast<NBTBase>(readNumber());
	}
	else switch(look_)
	{
		case '"':
			p = std::dynamic_pointer_cast<NBTBase>(readString());
			break;
		case 't':
		case 'f':
			p = std::dynamic_pointer_cast<NBTBase>(readBoolean());
			break;
		case '{':
			p = std::dynamic_pointer_cast<NBTBase>(readObject());
			break;
		case '[':
			p = std::dynamic_pointer_cast<NBTBase>(readArray());
			break;
		default:
			std::cerr << "THERES AN INVALID LOOKAHAEAD!" << std::endl << "ITS " << look_ << std::endl;
			std::cerr << "THAT'S IN ASCII '" << (char)look_ << "'" << std::endl;
			assert(false);
	}

	return p;
}

TAG_Compound_ptr_t JSON::readObject(void)
{
	assertNext('{');

	TAG_Compound_ptr_t nbt = Make<TAG_Compound>();

	while(true)
	{
		std::string name(readName());

		nbt->setTag(name, readAny());

		if(look_ != ',')
		{
			assertNext('}');
			break;
		}
		else
		{
			next();
		}
	}

	return nbt;
}

TAG_List_ptr_t JSON::readArray(void)
{
	assertNext('[');

	TAG_List_ptr_t nbt = Make<TAG_List>();

	while(true)
	{
		nbt->addTag(readAny());

		if(look_ != ',')
		{
			assertNext(']');
			break;
		}
		else
		{
			next();
		}
	}

	return nbt;
}

// # ---------------------------------------------------------------------------

void JSON::write(NBTBase& nbt, bool nl)
{
	NBTBase *p = &nbt;

	switch(nbt.getID())
	{
		case TAG_Byte::ID:
			write(*dynamic_cast<TAG_Byte *>(p));
			break;
		case TAG_Short::ID:
			write(*dynamic_cast<TAG_Short *>(p));
			break;
		case TAG_Int::ID:
			write(*dynamic_cast<TAG_Int *>(p));
			break;
		case TAG_Long::ID:
			write(*dynamic_cast<TAG_Long *>(p));
			break;
		case TAG_Float::ID:
			write(*dynamic_cast<TAG_Float *>(p));
			break;
		case TAG_Double::ID:
			write(*dynamic_cast<TAG_Double *>(p));
			break;
		case TAG_Byte_Array::ID:
			write(*dynamic_cast<TAG_Byte_Array *>(p));
			break;
		case TAG_String::ID:
			write(*dynamic_cast<TAG_String *>(p));
			break;
		case TAG_List::ID:
			write(*dynamic_cast<TAG_List *>(p));
			break;
		case TAG_Compound::ID:
			write(*dynamic_cast<TAG_Compound *>(p), nl);
			break;
		case TAG_Int_Array::ID:
			write(*dynamic_cast<TAG_Int_Array *>(p));
			break;
	}
}

void JSON::write(TAG_Byte& nbt)
{
	(*os_) << (nbt.get() ? "true" : "false");
}

void JSON::write(TAG_Short& nbt)
{
	writeNumber(nbt.getID(), nbt.get());
}

void JSON::write(TAG_Int& nbt)
{
	writeNumber(nbt.getID(), nbt.get());
}

void JSON::write(TAG_Long& nbt)
{
	(*os_) << nbt.get();
}

void JSON::write(TAG_Float& nbt)
{
	writeNumber(nbt.getID(), nbt.get());
}

void JSON::write(TAG_Double& nbt)
{
	writeNumber(nbt.getID(), nbt.get());
}

void JSON::write(TAG_Byte_Array& nbt)
{
	writeArray(nbt);
}

void JSON::write(TAG_String& nbt)
{
	(*os_) << '"' << nbt.get() << '"';
}

void JSON::write(TAG_List& nbt)
{
	bool f = true;

	newLine();

	(*os_) << '[';

	++ws_;

	for(auto p : nbt)
	{
		if(!f) (*os_) << ", ";

		newLine();

		write(*p, false);

		f = false;
	}

	--ws_;
	
	newLine();

	(*os_) << ']';
}

void JSON::write(TAG_Compound& nbt, bool nl)
{
	bool f = true;

	if(nl) newLine();

	(*os_) << '{';

	++ws_;

	for(auto p : nbt.Tags)
	{
		if(!f) (*os_) << ", ";

		newLine();

		writeName(p->getName());

		write(*p);

		f = false;
	}

	--ws_;

	newLine();

	(*os_) << '}';
}

void JSON::write(TAG_Int_Array& nbt)
{
	writeArray(nbt);
}


}}

