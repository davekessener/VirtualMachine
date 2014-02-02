#ifndef NBT_H
#define NBT_H

#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <string.h>

namespace NBT
{
	typedef std::uint8_t   BYTE;
	typedef std::uint16_t  WORD;
	typedef std::uint32_t DWORD;
	typedef std::uint64_t QWORD;

// # ===========================================================================

	class nbtistream
	{
		public:
			nbtistream(std::istream& _is) : is(_is) { }
			operator std::istream&( ) { return is; }
			template<typename T>
			T read( );
			nbtistream& read(BYTE *d, size_t n)
			{
				is.read(reinterpret_cast<char *>(d), n);
				return *this;
			}
		private:
			std::istream &is;
	};

// # ---------------------------------------------------------------------------

	class nbtostream
	{
		public:
			nbtostream(std::ostream& _os) : os(_os) { }
			operator std::ostream&( ) { return os; }
			template<typename T>
			nbtostream& write(T t);
			nbtostream& write(BYTE *d, size_t n)
			{
				os.write(reinterpret_cast<char *>(d), n);
				return *this;
			}
		private:
			std::ostream &os;
	};

// # ===========================================================================

	class _NBTBase
	{
		public:
			void write(std::ostream&);
			void read(std::istream&);
			virtual BYTE getID( ) = 0;
		protected:
			virtual void _write(nbtostream&) = 0;
			virtual void _read(nbtistream&) = 0;
			BYTE *name;

			template<BYTE> friend class NBTList;
	};

// # ---------------------------------------------------------------------------

	template<BYTE id>
	class NBTBase : public _NBTBase
	{
		public:
			inline BYTE getID(void)
			{
				return id;
			}
		protected:
			static const BYTE ID = id;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T>
	class NBTSimple : public NBTBase<ID>
	{
		public:
			inline NBTSimple(T t) { init("", t); }
			inline NBTSimple(const char *s = "", T t = T()) { init(s, t); }
		protected:
			void _write(nbtostream&);
			void _read(nbtistream&);
		private:
			void init(const char *, T);
			T value;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	class NBTArray : public NBTBase<ID>
	{
		public:
			inline NBTArray(T1 l, T2 *v = NULL) { init(NULL, l, v); }
			inline NBTArray(const char *s = NULL, T1 l = 0, const T2 *v = NULL) { init(s, l, v); }
			inline NBTArray(std::initializer_list<T2> v) { init(NULL, v); }
			inline NBTArray(const char *s, std::initializer_list<T2> v) { init(s, v); }
		protected:
			void _write(nbtostream&);
			void _read(nbtistream&);
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
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTList : public NBTBase<ID>
	{
		public:
			inline NBTList(DWORD l, _NBTBase **v = NULL) { init("", 0, l, v); }
			inline NBTList(const char *s, DWORD l, _NBTBase **v) { init(s, 0, l, v); }
			inline NBTList(const char *s = "", BYTE id = 0, DWORD l = 0, _NBTBase **v = NULL) { init(s, id, l, v); }
			inline NBTList(std::initializer_list<_NBTBase *> v) { init("", v); }
			inline NBTList(const char *s, std::initializer_list<_NBTBase *> v) { init(s, v); }
		protected:
			void _write(nbtostream&);
			void _read(nbtistream&);
		private:
			void init(const char *, std::initializer_list<_NBTBase *>);
			void init(const char *, BYTE, DWORD, _NBTBase **);
			BYTE tagIds;
			DWORD length;
			_NBTBase **tags;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTTagCompound : public NBTBase<ID>
	{
		public:
			inline NBTTagCompound(const char *s = "") { init(s, 0, NULL); }
			inline NBTTagCompound(int l, _NBTBase **v) { init("", l, v); }
			inline NBTTagCompound(const char *s, int l, _NBTBase **v) { init(s, l, v); }
			inline NBTTagCompound(std::initializer_list<_NBTBase *> v) { init("", v); }
			inline NBTTagCompound(const char *s, std::initializer_list<_NBTBase *> v) { init(s, v); }
		protected:
			void _write(nbtostream&);
			void _read(nbtistream&);
		private:
			void init(const char *, int, _NBTBase **);
			void init(const char *, std::initializer_list<_NBTBase *>);
			int length;
			_NBTBase **tags;
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

// # ===========================================================================

	void _NBTBase::write(std::ostream& _os)
	{
		nbtostream os(_os);

		os.write(getID()).write<WORD>(strlen(reinterpret_cast<char *>(name)));

		if(*name)
		{
			os.write(name, strlen(reinterpret_cast<char *>(name)));
		}

		_write(os);
	}

	void _NBTBase::read(std::istream& _is)
	{
		nbtistream is(_is);

		size_t l = is.read<WORD>();
		name = new BYTE[l + 1];
		if(l) is.read(name, l);
		name[l] = '\0';

		_read(is);
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::_write(nbtostream& os)
	{
		os.write(value);
	}

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::_read(nbtistream& is)
	{
		value = is.read<T>();
	}

	template<BYTE ID, typename T>
	void NBTSimple<ID, T>::init(const char *s, T t)
	{
		_NBTBase::name = (BYTE *) strdup(s);
		value = t;
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::_write(nbtostream& os)
	{
		os.write(length);

		for(int i = 0 ; i < length ; i++)
		{
			os.write(values[i]);
		}
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::_read(nbtistream& is)
	{
		length = is.read<T1>();
		values = new T2[length];

		for(int i = 0 ; i < length ; i++)
		{
			values[i] = is.read<T2>();
		}
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::init(const char *s, std::initializer_list<T2> v)
	{
		_NBTBase::name = (BYTE *) strdup(s);
		T2 *t = values = new T2[v.size()];

		for(T2 t2 : v)
		{
			*t++ = t2;
		}

		length = t - values;
	}

	template<BYTE ID, typename T1, typename T2>
	void NBTArray<ID, T1, T2>::init(const char *s, T1 l, const T2 *v)
	{
		_NBTBase::name = (BYTE *) strdup(s);
		length = l;
		values = l ? new T2[l] : NULL;
		if(l && v) memcpy(values, v, l * sizeof(T2));
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	void NBTList<ID>::_write(nbtostream& os)
	{
		assert(tagIds);
		os.write(tagIds).write(length);

		for(int i = 0 ; i < length ; i++)
		{
			assert(tagIds == tags[i]->getID());
			tags[i]->_write(os);
		}
	}

	template<BYTE ID>
	void NBTList<ID>::_read(nbtistream& is)
	{
	}

	template<BYTE ID>
	void NBTList<ID>::init(const char *s, std::initializer_list<_NBTBase *> v)
	{
		_NBTBase::name = (BYTE *) strdup(s);
		_NBTBase **t = tags = new _NBTBase*[v.size()];

		for(_NBTBase *nbt : v)
		{
			*t++ = nbt;
		}

		tagIds = tags[0]->getID();
		length = t - tags;
	}

	template<BYTE ID>
	void NBTList<ID>::init(const char *s, BYTE id, DWORD l, _NBTBase **v)
	{
		_NBTBase::name = (BYTE *) strdup(s);
		length = l;
		tagIds = id;
		tags = l ? new _NBTBase*[l] : NULL;
		if(l && v) memcpy(tags, v, l * sizeof(_NBTBase *));

		if(l && v) tagIds = v[0]->getID();
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	void NBTTagCompound<ID>::_write(nbtostream& os)
	{
		for(int i = 0 ; i < length ; i++)
		{
			tags[i]->write(os);
		}

		os.write<BYTE>(0);
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::_read(nbtistream& is)
	{
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::init(const char *s, std::initializer_list<_NBTBase *> v)
	{
		_NBTBase::name = (BYTE *) strdup(s);
		_NBTBase **t = tags = new _NBTBase*[v.size()];

		for(_NBTBase *nbt : v)
		{
			*t++ = nbt;
		}

		length = t - tags;
	}

	template<BYTE ID>
	void NBTTagCompound<ID>::init(const char *s, int l, _NBTBase **v)
	{
		_NBTBase::name = (BYTE *) strdup(s);
		length = l;
		tags = l ? new _NBTBase*[l] : NULL;
		if(l && v) memcpy(tags, v, l * sizeof(_NBTBase *));
	}

// # ---------------------------------------------------------------------------

	template<typename T>
	T nbtistream::read(void)
	{
		T t;
		is.read(reinterpret_cast<char *>(&t), sizeof(T));
		std::reverse(reinterpret_cast<BYTE *>(&t), reinterpret_cast<BYTE *>(&t) + sizeof(T));
		return t;
	}

	template<typename T>
	nbtostream& nbtostream::write(T t)
	{
		std::reverse(reinterpret_cast<BYTE *>(&t), reinterpret_cast<BYTE *>(&t) + sizeof(T));
		os.write(reinterpret_cast<char *>(&t), sizeof(T));

		return *this;
	}
}

#endif

