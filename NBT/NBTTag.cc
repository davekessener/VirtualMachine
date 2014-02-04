#define NBT_IMP
#include "NBTTag.h"
#undef NBT_IMP

namespace NBT
{

	void NBTBase::write(std::ostream& os)
	{
		write(nbt_std_writer(os));
	}

	void NBTBase::write(gzip::ogzstream& os)
	{
		write(nbt_gzip_writer(os));
	}

	void NBTBase::write(const nbtostream& os)
	{
		os.write(getID());
		os.write<WORD>(strlen(reinterpret_cast<char *>(name)));

		if(*name)
		{
			os.write(name, strlen(reinterpret_cast<char *>(name)));
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

	void NBTBase::setName(const char *s)
	{
		if(name) free(name);
		name = reinterpret_cast<BYTE *>(strdup(s));
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::_write(const nbtostream& os)
	{
		os.write(value);
	}

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::_read(const nbtistream& is)
	{
		value = is.read<T>();
	}

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::init(const char *s, T t)
	{
		NBTBase::setName(s);
		value = t;
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::_write(const nbtostream& os)
	{
		os.write(length);

		for(int i = 0 ; i < length ; i++)
		{
			os.write(values[i]);
		}
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::_read(const nbtistream& is)
	{
		if(values) delete[] values;

		length = is.read<T1>();
		values = length ? new T2[length] : NULL;

		for(int i = 0 ; i < length ; i++)
		{
			values[i] = is.read<T2>();
		}
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::init(const char *s, std::initializer_list<T2> v)
	{
		NBTBase::setName(s);

		delete[] values;

		T2 *t = values = v.size() ? new T2[v.size()] : NULL;

		for(T2 t2 : v)
		{
			*t++ = t2;
		}

		length = t - values;
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::init(const char *s, T1 l, const T2 *v)
	{
		NBTBase::setName(s);

		delete[] values;

		length = l;
		values = l ? new T2[l] : NULL;
		if(l && v) memcpy(values, v, l * sizeof(T2));
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	void NBTList<ID>::_write(const nbtostream& os)
	{
		os.write<BYTE>(tagIds ? tagIds : 1);
		os.write<DWORD>(length);

		for(int i = 0 ; i < length ; i++)
		{
			assert(tagIds == tags[i]->getID());

			tags[i]->_write(os);
		}
	}

	template<BYTE ID>
	void NBTList<ID>::_read(const nbtistream& is)
	{
		tagIds = is.read<BYTE>();
		length = is.read<decltype(length)>();

		if(tags) delete[] tags;

		tags = length ? new NBT_ptr_t[length] : NULL;

		for(int i = 0 ; i < length ; i++)
		{
			tags[i] = NBT_ptr_t(NBTHelper::Instance().Default(tagIds));
			tags[i]->_read(is);
		}
	}

	template<BYTE ID>
	void NBTList<ID>::init(const char *s, std::initializer_list<NBT_ptr_t> v)
	{
		NBTBase::setName(s);

		delete[] tags;

		NBT_ptr_t *t = tags = v.size() ? new NBT_ptr_t[v.size()] : NULL;

		tagIds = v.size() ? (*v.begin())->getID() : 0;
		for(NBT_ptr_t nbt : v)
		{
			assert(tagIds == nbt->getID());

			*t++ = nbt;
		}

		length = t - tags;
	}

	template<BYTE ID>
	void NBTList<ID>::init(const char *s, BYTE id, DWORD l, NBT_ptr_t *v)
	{
		NBTBase::setName(s);

		assert(id < 12);

		delete[] tags;

		length = l;
		tagIds = id;
		tags = l ? new NBT_ptr_t[l] : NULL;

		if(v && l)
		{
			tagIds = (*v)->getID();
			for(int i = 0 ; i < l ; i++)
			{
				assert(tagIds == v[i]->getID());

				tags[i] = v[i];
			}
		}
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	void NBTTagCompound<ID>::_write(const nbtostream& os)
	{
		for(int i = 0 ; i < length ; i++)
		{
			tags[i]->write(os);
		}

		os.write<BYTE>(0);
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::_read(const nbtistream& is)
	{
		std::vector<NBTBase *> tmp;

		while(true)
		{
			NBTBase *b = NBTHelper::Instance().Read(is);

			if(!b) break;

			tmp.push_back(b);
		}

		delete[] tags;

		NBT_ptr_t *t = tags = tmp.size() ? new NBT_ptr_t[tmp.size()] : NULL;
		for(NBTBase *nbt : tmp)
		{
			*t++ = NBT_ptr_t(nbt);
		}

		length = t - tags;
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::init(const char *s, std::initializer_list<NBT_ptr_t> v)
	{
		NBTBase::setName(s);

		delete[] tags;

		NBT_ptr_t *t = tags = v.size() ? new NBT_ptr_t[v.size()] : NULL;

		for(NBT_ptr_t nbt : v)
		{
			*t++ = nbt;
		}

		length = t - tags;
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::init(const char *s, int l, NBT_ptr_t *v)
	{
		NBTBase::setName(s);

		delete[] tags;

		length = l;
		tags = l ? new NBT_ptr_t[l] : NULL;

		if(l && v)
		{
			for(int i = 0 ; i < l ; i++)
			{
				tags[i] = v[i];
			}
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

