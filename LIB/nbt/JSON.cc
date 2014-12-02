#include "JSON.h"

namespace nbt { namespace json {

class JSON
{
	public:
		JSON(std::ostream& o) : os_(o), ws_(0) { }
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
	private:
		void newLine( )
			{ if(ws_ >= 0) { os_ << '\n'; for(int i(ws_) ; i ; --i) os_ << '\t'; } }
		void writeName(const std::string& s)
			{ os_ << '"' << s << "\" : "; }
		template<typename T> void writeNumber(BYTE id, const T& v)
			{ os_ << "{ \"type\" : " << id << ", \"value\" : " << v << " }"; }
		template<typename T> void writeArray(const T& a)
		{
			bool f = true;
			os_ << "{ \"type\" : " << T::ID << ", "
				<< "\"value\" : [ ";
			
			for(const auto& v : a)
			{
				if(!f) os_ << ", ";
				os_ << v;
			}

			os_ << " }";
		}
	private:
		std::ostream &os_;
		int ws_ = 0;
};

TAG_Compound_ptr_t read(std::istream& is)
{
}

void write(std::ostream& os, TAG_Compound_ptr_t nbt)
{
	JSON writer(os);

	writer.write(*nbt, false);

	os << '\n' << std::endl;
}

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
	os_ << (nbt.get() ? "true" : "false");
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
	os_ << nbt.get();
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
	os_ << '"' << nbt.get() << '"';
}

void JSON::write(TAG_List& nbt)
{
	bool f = true;

	newLine();

	os_ << '[';

	++ws_;

	for(auto p : nbt)
	{
		if(!f) os_ << ", ";

		newLine();

		write(*p, false);

		f = false;
	}

	--ws_;
	
	newLine();

	os_ << ']';
}

void JSON::write(TAG_Compound& nbt, bool nl)
{
	bool f = true;

	if(nl) newLine();

	os_ << '{';

	++ws_;

	for(auto p : nbt.Tags)
	{
		if(!f) os_ << ", ";

		newLine();

		writeName(p->getName());

		write(*p);

		f = false;
	}

	--ws_;

	newLine();

	os_ << '}';
}

void JSON::write(TAG_Int_Array& nbt)
{
	writeArray(nbt);
}


}}

