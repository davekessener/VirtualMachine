#define NBT_IMP
#include "NBTMain.h"
#undef NBT_IMP

namespace nbt
{
// # ===========================================================================

	void NBTBase::write(std::ostream& os) const
	{
		write(nbt_std_writer(os));
	}

	void NBTBase::write(gzip::ogzstream& os) const
	{
		write(nbt_gzip_writer(os));
	}

	void NBTBase::write(const nbtostream& os) const
	{
		os.write<BYTE>(getID());
		os.write<WORD>(strlen(reinterpret_cast<const char *>(name)));

		if(*name)
		{
			os.write(name, strlen(reinterpret_cast<const char *>(name)));
		}

		_write(os);
	}

	void NBTBase::read(std::istream& is)
	{
		read(nbt_std_reader(is));
	}

	void NBTBase::read(gzip::igzstream& is)
	{
		read(nbt_gzip_reader(is));
	}

	void NBTBase::read(const nbtistream& is)
	{
		size_t l = is.read<WORD>();
		if(name) free(name);
		name = (BYTE *) std::malloc(l + 1);
		if(l) is.read(name, l);
		name[l] = '\0';

		_read(is);
	}

	void NBTBase::setName(const std::string& s)
	{
		if(name) free(name);
		name = reinterpret_cast<BYTE *>(strdup(s.c_str()));
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::_write(const nbtostream& os) const
	{
		os.write<T>(value);
	}

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::_read(const nbtistream& is)
	{
		value = is.read<T>();
	}

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::init(const std::string& s, T t)
	{
		NBTBase::setName(s);
		value = t;
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::_write(const nbtostream& os) const
	{
		os.write<T1>(static_cast<T1>(vec_t::size()));

		for(T2 v : *this)
		{
			os.write<T2>(v);
		}
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::_read(const nbtistream& is)
	{
		vec_t::clear();

		size_t length = static_cast<size_t>(is.read<T1>());
		vec_t::reserve(length);

		while(length--)
		{
			vec_t::push_back(is.read<T2>());
		}
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::init(const std::string& s, std::initializer_list<T2> v)
	{
		NBTBase::setName(s);

		vec_t::clear();

		vec_t::reserve(v.size());

		for(T2 t2 : v)
		{
			vec_t::push_back(t2);
		}
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	void NBTList<ID>::_write(const nbtostream& os) const
	{
		os.write<BYTE>(tagIds ? tagIds : 1);
		os.write<DWORD>(static_cast<DWORD>(vec_t::size()));

		for(auto i = vec_t::cbegin() ; i != vec_t::cend() ; ++i)
		{
			const NBT_ptr_t nbt = *i;

			assert(tagIds == nbt->getID());

			nbt->_write(os);
		}
	}

	template<BYTE ID>
	void NBTList<ID>::_read(const nbtistream& is)
	{
		tagIds = is.read<BYTE>();
		size_t length = static_cast<size_t>(is.read<DWORD>());

		vec_t::clear();
		vec_t::reserve(length);

		while(length--)
		{
			NBT_ptr_t nbt(NBTHelper::Instance().Default(tagIds));
			nbt->_read(is);
			vec_t::push_back(nbt);
		}
	}

	template<BYTE ID>
	void NBTList<ID>::init(const std::string& s, std::initializer_list<NBT_ptr_t> v)
	{
		NBTBase::setName(s);

		vec_t::clear();
		vec_t::reserve(v.size());

		tagIds = v.size() ? (*v.begin())->getID() : 0;
		for(NBT_ptr_t nbt : v)
		{
			assert(tagIds == nbt->getID());

			vec_t::push_back(nbt);
		}
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	void NBTTagCompound<ID>::_write(const nbtostream& os) const
	{
		for(auto i = map_t::cbegin() ; i != map_t::cend() ; ++i)
		{
			i->second->write(os);
		}

		os.write<BYTE>(0);
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::_read(const nbtistream& is)
	{
		map_t::clear();

		while(true)
		{
			NBTBase *b = NBTHelper::Instance().Read(is);

			if(!b) break;

			assert(b->getName().length());

			map_t::operator[](b->getName()) = NBT_ptr_t(b);
		}
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::init(const std::string& name, std::initializer_list<NBT_ptr_t> v)
	{
		NBTBase::setName(name);

		map_t::clear();

		for(NBT_ptr_t nbt : v)
		{
			std::string s = nbt->getName();

			assert(s.length());

			map_t::operator[](s) = nbt;
		}
	}

// # ===========================================================================

	NBTBase *NBTHelper::Default(BYTE id)
	{
		if(!id) return NULL;

		assert(id < 12);

		return cons[id]();
	}

	NBT_ptr_t NBTHelper::Read(std::istream& is)
	{
		NBTBase *nbt = Read(nbt_std_reader(is));

		assert(nbt);

		return NBT_ptr_t(nbt);
	}

	NBT_ptr_t NBTHelper::Read(gzip::igzstream& is)
	{
		NBTBase *nbt = Read(nbt_gzip_reader(is));

		assert(nbt);

		return NBT_ptr_t(nbt);
	}

	NBTBase *NBTHelper::Read(const nbtistream& is)
	{
		BYTE id = is.read<BYTE>();

		NBTBase *nbt = Default(id);

		if(nbt) nbt->read(is);

		return nbt;
	}

	NBTHelper& NBTHelper::Instance(void)
	{
		static NBTHelper nbth;
		return nbth;
	}

	NBTHelper::NBTHelper(void)
	{
		cons[TAG_Byte::ID]       = []()->NBTBase * { return new TAG_Byte; };
		cons[TAG_Short::ID]      = []()->NBTBase * { return new TAG_Short; };
		cons[TAG_Int::ID]        = []()->NBTBase * { return new TAG_Int; };
		cons[TAG_Long::ID]       = []()->NBTBase * { return new TAG_Long; };
		cons[TAG_Float::ID]      = []()->NBTBase * { return new TAG_Float; };
		cons[TAG_Double::ID]     = []()->NBTBase * { return new TAG_Double; };
		cons[TAG_Byte_Array::ID] = []()->NBTBase * { return new TAG_Byte_Array; };
		cons[TAG_String::ID]     = []()->NBTBase * { return new TAG_String; };
		cons[TAG_List::ID]       = []()->NBTBase * { return new TAG_List; };
		cons[TAG_Compound::ID]   = []()->NBTBase * { return new TAG_Compound; };
		cons[TAG_Int_Array::ID]  = []()->NBTBase * { return new TAG_Int_Array; };
	}

	NBTHelper::~NBTHelper(void)
	{
	}

// # ===========================================================================

	template class NBTSimple<1, BYTE>; 
	template class NBTSimple<2, WORD>;
	template class NBTSimple<3, DWORD>;
	template class NBTSimple<4, QWORD>;
	template class NBTSimple<5, float>;
	template class NBTSimple<6, double>;
	template class NBTArray<7, DWORD, BYTE>;
	template class NBTString<8>;
	template class NBTList<9>;
	template class NBTTagCompound<10>;
	template class NBTArray<11, DWORD, DWORD>;
}

