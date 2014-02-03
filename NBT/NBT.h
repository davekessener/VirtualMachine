#ifndef NBT_H
#define NBT_H

#include <iostream>
#include <initializer_list>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <string.h>
#include "gzstream.h"
#include "Reader.h"
#include "Writer.h"

namespace NBT
{
	typedef std::uint8_t   BYTE;
	typedef std::uint16_t  WORD;
	typedef std::uint32_t DWORD;
	typedef std::uint64_t QWORD;

	typedef basic_nbt_reader nbtistream;
	typedef basic_nbt_writer nbtostream;

// # ===========================================================================

	class NBTBase
	{
		public:
			NBTBase(void) : name(NULL) { std::cerr << "!!! Base constructor" << std::endl; }
			virtual ~NBTBase(void)
			{
				std::cerr << "!!! Base destructor of '" << NBTBase::name << "'\n";
				free(name);
			}
			void write(std::ostream&);
			void write(gzip::ogzstream&);
			void write(nbtostream);
			void read(std::istream&);
			void read(gzip::igzstream&);
			void read(nbtistream);
			virtual BYTE getID( ) = 0;
		protected:
			void setName(const char *);
			virtual void _write(nbtostream) = 0;
			virtual void _read(nbtistream) = 0;
			BYTE *name;

			template<BYTE> friend class NBTList;
	};

	typedef std::shared_ptr<NBTBase> NBT_ptr_t;

// # ---------------------------------------------------------------------------

	template<BYTE id>
	class _NBTBase : public NBTBase
	{
		public:
			inline BYTE getID(void)
			{
				return id;
			}
			static const BYTE ID;
	};

	template<BYTE id>
	const BYTE _NBTBase<id>::ID = id;

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T>
	class NBTSimple : public _NBTBase<ID>
	{
		public:
			inline NBTSimple(T t) { init("", t); }
			inline NBTSimple(const char *s = "", T t = T()) { init(s, t); }
		protected:
			void _write(nbtostream);
			void _read(nbtistream);
		private:
			void init(const char *, T);
			T value;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	class NBTArray : public _NBTBase<ID>
	{
		public:
			inline NBTArray(T1 l, T2 *v = NULL) : values(NULL) { init(NULL, l, v); }
			inline NBTArray(const char *s = "", T1 l = 0, const T2 *v = NULL) : values(NULL) { init(s, l, v); }
			inline NBTArray(std::initializer_list<T2> v) : values(NULL) { init("", v); }
			inline NBTArray(const char *s, std::initializer_list<T2> v) : values(NULL) { init(s, v); }
			inline ~NBTArray(void)
			{
				std::cerr << "! Array destructor of '" << NBTBase::name << "'\n";
				delete[] values;
			}
		protected:
			void _write(nbtostream);
			void _read(nbtistream);
		private:
			void init(const char *, std::initializer_list<T2>);
			void init(const char *, T1, const T2 *);
			T1 length;
			T2 *values;
	};

	template<BYTE ID>
	class NBTString : public NBTArray<ID, WORD, BYTE>
	{
		public:
			inline NBTString(const char *s, const char *v) : 
				NBTArray<ID, WORD, BYTE>(s, (WORD) (v ? strlen(v) : 0), reinterpret_cast<const BYTE *>(v)) { }
			inline NBTString( ) : NBTArray<ID, WORD, BYTE>("", 0, NULL) { }
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTList : public _NBTBase<ID>
	{
		public:
			inline NBTList(DWORD l, NBT_ptr_t *v = NULL) : tags(NULL) { init("", 0, l, v); }
			inline NBTList(const char *s, DWORD l, NBT_ptr_t *v) : tags(NULL) { init(s, 0, l, v); }
			inline NBTList(const char *s = "", BYTE id = 0, DWORD l = 0, NBT_ptr_t *v = NULL) : tags(NULL) { init(s, id, l, v); }
			inline NBTList(std::initializer_list<NBT_ptr_t> v) : tags(NULL) { init("", v); }
			inline NBTList(const char *s, std::initializer_list<NBT_ptr_t> v) : tags(NULL) { init(s, v); }
			inline ~NBTList(void)
			{
				std::cerr << "! List destructor of '" << NBTBase::name << "'\n";

				delete[] tags;
			}
		protected:
			void _write(nbtostream);
			void _read(nbtistream);
		private:
			void init(const char *, std::initializer_list<NBT_ptr_t>);
			void init(const char *, BYTE, DWORD, NBT_ptr_t *);
			BYTE tagIds;
			DWORD length;
			NBT_ptr_t *tags;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTTagCompound : public _NBTBase<ID>
	{
		public:
			inline NBTTagCompound(const char *s = "") : tags(NULL) { init(s, 0, NULL); }
			inline NBTTagCompound(int l, NBT_ptr_t *v) : tags(NULL) { init("", l, v); }
			inline NBTTagCompound(const char *s, int l, NBT_ptr_t *v) : tags(NULL) { init(s, l, v); }
			inline NBTTagCompound(std::initializer_list<NBT_ptr_t> v) : tags(NULL) { init("", v); }
			inline NBTTagCompound(const char *s, std::initializer_list<NBT_ptr_t> v) : tags(NULL) { init(s, v); }
			inline ~NBTTagCompound(void)
			{
				std::cerr << "! Compound destructor of '" << NBTBase::name << "'\n";

				delete[] tags;
			}
		protected:
			void _write(nbtostream);
			void _read(nbtistream);
		private:
			void init(const char *, int, NBT_ptr_t *);
			void init(const char *, std::initializer_list<NBT_ptr_t>);
			int length;
			NBT_ptr_t *tags;
	};

// # ---------------------------------------------------------------------------

	typedef NBTSimple		<1, BYTE> 			TAG_Byte;
	typedef NBTSimple		<2, WORD> 			TAG_Short;
	typedef NBTSimple		<3, DWORD> 			TAG_Int;
	typedef NBTSimple		<4, QWORD> 			TAG_Long;
	typedef NBTSimple		<5, float>	 		TAG_Float;
	typedef NBTSimple		<6, double> 		TAG_Double;
	typedef NBTArray		<7, DWORD, BYTE> 	TAG_Byte_Array;
	typedef NBTString		<8> 				TAG_String;
	typedef NBTList			<9> 				TAG_List;
	typedef NBTTagCompound	<10> 				TAG_Compound;
	typedef NBTArray		<11, DWORD, DWORD> 	TAG_Int_Array;

// # ---------------------------------------------------------------------------

	class NBTHelper
	{
		public:
			NBT_ptr_t Read(std::istream&);
			NBT_ptr_t Read(gzip::igzstream&);
			NBTBase *Default(BYTE);
			NBTBase *Read(nbtistream);
			static NBTHelper& instance( );
		private:
			NBTHelper( );
			~NBTHelper( );
			NBTHelper(const NBTHelper&);
			NBTHelper& operator=(const NBTHelper&);

			std::map<BYTE, std::function<NBTBase *(void)>> cons;
	};

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

	NBTBase *NBTHelper::Read(nbtistream is)
	{
		BYTE id = is.read<BYTE>();

		NBTBase *nbt = Default(id);

		if(nbt) nbt->read(is);

		return nbt;
	}

	NBTHelper& NBTHelper::instance(void)
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

	void NBTBase::write(std::ostream& os)
	{
		write(nbtostream(os));
	}

	void NBTBase::write(nbtostream os)
	{
		os.write(getID()).write<WORD>(strlen(reinterpret_cast<char *>(name)));

		if(*name)
		{
			os.write(name, strlen(reinterpret_cast<char *>(name)));
		}

		_write(os);
	}

	void NBTBase::read(std::istream& is)
	{
		read(nbtistream(is));
	}

	void NBTBase::read(nbtistream is)
	{
		size_t l = is.read<WORD>();
		if(name) free(name);
		name = (BYTE *) std::malloc(l + 1);
		if(l) is.read(name, l);
		name[l] = '\0';

		std::cerr << "# Reading tag named '" << name << "'\n";

		_read(is);
	}

	void NBTBase::setName(const char *s)
	{
		if(name) free(name);
		name = reinterpret_cast<BYTE *>(strdup(s));
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::_write(nbtostream os)
	{
		os.write(value);
	}

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::_read(nbtistream is)
	{
		value = is.read<T>();
		std::cerr << "# Reading simple value " << value << std::endl;
	}

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::init(const char *s, T t)
	{
		NBTBase::setName(s);
		value = t;
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::_write(nbtostream os)
	{
		os.write(length);

		for(int i = 0 ; i < length ; i++)
		{
			os.write(values[i]);
		}
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::_read(nbtistream is)
	{
		if(values) delete[] values;

		length = is.read<T1>();
		values = length ? new T2[length] : NULL;

		std::cerr << "# Read " << (int)length << " values:" << std::endl;

		for(int i = 0 ; i < length ; i++)
		{
			values[i] = is.read<T2>();
			std::cerr << "##\t" << values[i] << std::endl;
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
	void NBTList<ID>::_write(nbtostream os)
	{
		os.write(tagIds ? tagIds : 1).write(length);

		for(int i = 0 ; i < length ; i++)
		{
			assert(tagIds == tags[i]->getID());

			tags[i]->_write(os);
		}
	}

	template<BYTE ID>
	void NBTList<ID>::_read(nbtistream is)
	{
		tagIds = is.read<BYTE>();
		length = is.read<decltype(length)>();

		if(tags) delete[] tags;

		tags = length ? new NBT_ptr_t[length] : NULL;

		std::cerr << "# Reading list(" << (int)length << ") of type " << (int)tagIds << ":\n";

		for(int i = 0 ; i < length ; i++)
		{
			tags[i] = NBT_ptr_t(NBTHelper::instance().Default(tagIds));
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
	void NBTTagCompound<ID>::_write(nbtostream os)
	{
		for(int i = 0 ; i < length ; i++)
		{
			tags[i]->write(os);
		}

		os.write<BYTE>(0);
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::_read(nbtistream is)
	{
		std::vector<NBTBase *> tmp;

		std::cerr << "# Reading tag compound" << std::endl;

		while(true)
		{
			NBTBase *b = NBTHelper::instance().Read(is);
			if(!b) break;
			std::cerr << "##\tRead tag of type " << (int)b->getID() << std::endl;
			tmp.push_back(b);
		}

		std::cerr << "##\t[DONE]\n";

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
}

//// # ===========================================================================
//
//	class nbtistream
//	{
//		public:
//			nbtistream(std::istream& _is) : is(_is) { }
//			template<typename T>
//			T read( );
//			nbtistream& read(BYTE *d, size_t n)
//			{
//				is.read(reinterpret_cast<char *>(d), n);
//				return *this;
//			}
//		private:
//			std::istream &is;
//	};
//
//	class nbtostream
//	{
//		public:
//			nbtostream(std::ostream& _os) : os(_os) { }
//			template<typename T>
//			nbtostream& write(T t);
//			nbtostream& write(BYTE *d, size_t n)
//			{
//				os.write(reinterpret_cast<char *>(d), n);
//				return *this;
//			}
//		private:
//			std::ostream &os;
//	};
//
//// # ---------------------------------------------------------------------------
//
//	template<typename T>
//	T nbtistream::read(void)
//	{
//		T t;
//		is.read(reinterpret_cast<char *>(&t), sizeof(T));
//
//#ifndef RETAIN_ENDIANNESS
//		std::reverse(reinterpret_cast<BYTE *>(&t), reinterpret_cast<BYTE *>(&t) + sizeof(T));
//#endif
//
//		return t;
//	}
//
//	template<typename T>
//	nbtostream& nbtostream::write(T t)
//	{
//#ifndef RETAIN_ENDIANNESS
//		std::reverse(reinterpret_cast<BYTE *>(&t), reinterpret_cast<BYTE *>(&t) + sizeof(T));
//#endif
//
//		os.write(reinterpret_cast<char *>(&t), sizeof(T));
//
//		return *this;
//	}

#endif

