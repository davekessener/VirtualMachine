#ifndef NBT_H
#define NBT_H

#include <iostream>
#include <initializer_list>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <string.h>
#include <GZ/gzstream.h>
#include "common.h"
#include "Reader.h"
#include "Writer.h"

namespace NBT
{
// # ===========================================================================

	class NBTBase
	{
		public:
			NBTBase(void) : name(NULL) { }
			virtual ~NBTBase(void)
			{
				free(name);
			}
			void write(std::ostream&);
			void write(gzip::ogzstream&);
  			void write(const nbtostream&);
			void read(std::istream&);
			void read(gzip::igzstream&);
			void read(const nbtistream&);
			void setName(const char *);
			inline const char *getName(void) { return reinterpret_cast<const char *>(name); }
			virtual BYTE getID( ) = 0;
		protected:
			virtual void _write(const nbtostream&) = 0;
			virtual void _read(const nbtistream&) = 0;
			BYTE *name;

			template<BYTE> friend class NBTList;
	};

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
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			void init(const char *, T);
			T value;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	class NBTArray : public _NBTBase<ID>
	{
		public:
			inline NBTArray(const char *s = "") : length(0), values(NULL) { NBTBase::setName(s); }
			template<typename T>
			inline NBTArray(const char *s, T t1, T t2) : length(0), values(NULL) { init(s, t1, t2); }
			inline NBTArray(const char *s, std::initializer_list<T2> v) : length(0), values(NULL) { init(s, v); }
			template<typename T>
			inline NBTArray(T t1, T t2) : length(0), values(NULL) { init("", t1, t2); }
			inline NBTArray(std::initializer_list<T2> v) : length(0), values(NULL) { init("", v); }
			inline ~NBTArray(void)
			{
				delete[] values;
			}
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			template<typename T>
			void init(const char *, T, T);
			void init(const char *, std::initializer_list<T2>);
			T1 length;
			T2 *values;
	};

	template<BYTE ID>
	class NBTString : public NBTArray<ID, WORD, BYTE>
	{
		public:
			inline NBTString(const char *s, const char *v) : 
				NBTArray<ID, WORD, BYTE>(
						s, 
						reinterpret_cast<const BYTE *>(v),
						reinterpret_cast<const BYTE *>(v ? v + strlen(v) : NULL)) { }
			inline NBTString(const char *s = "") : NBTArray<ID, WORD, BYTE>(s) { }
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTList : public _NBTBase<ID>
	{
		public:
			inline NBTList(const char *s = "", BYTE id = 0) : tags(NULL), tagIds(id) { NBTBase::setName(s); }
			template<typename T>
			inline NBTList(const char *s, T t1, T t2) : tags(NULL), tagIds(0) { init(s, t1, t2); }
			inline NBTList(const char *s, std::initializer_list<NBT_ptr_t> v) : tags(NULL), tagIds(0) { init(s, v); }
			inline NBTList(BYTE id) : tags(NULL), tagIds(id) { NBTBase::setName(""); }
			template<typename T>
			inline NBTList(T t1, T t2) : tags(NULL), tagIds(0) { init("", t1, t2); }
			inline NBTList(std::initializer_list<NBT_ptr_t> v) : tags(NULL), tagIds(0) { init("", v); }
			inline ~NBTList(void)
			{
				delete[] tags;
			}
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			template<typename T>
			void init(const char *, T, T);
			void init(const char *, std::initializer_list<NBT_ptr_t>);
			BYTE tagIds;
			DWORD length;
			NBT_ptr_t *tags;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTTagCompound : public _NBTBase<ID>
	{
		public:
			inline NBTTagCompound(const char *s = "") { NBTBase::setName(s); }
			template<typename T>
			inline NBTTagCompound(const char *s, T t1, T t2) { init(s, t1, t2); }
			inline NBTTagCompound(const char *s, std::initializer_list<NBT_ptr_t> v) { init(s, v); }
			template<typename T>
			inline NBTTagCompound(T t1, T t2) { init("", t1, t2); }
			inline NBTTagCompound(std::initializer_list<NBT_ptr_t> v) { init("", v); }
			inline ~NBTTagCompound(void)
			{
			}
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			void init(const char *);
			template<typename T>
			void init(const char *, T, T);
			void init(const char *, std::initializer_list<NBT_ptr_t>);
			std::map<std::string, NBT_ptr_t> tags;
	};

// # ---------------------------------------------------------------------------

	class NBTHelper
	{
		public:
			NBT_ptr_t Read(std::istream&);
			NBT_ptr_t Read(gzip::igzstream&);
			NBTBase *Default(BYTE);
			NBTBase *Read(const nbtistream&);
			static NBTHelper& Instance( );
		private:
			NBTHelper( );
			~NBTHelper( );
			NBTHelper(const NBTHelper&);
			NBTHelper& operator=(const NBTHelper&);

			std::map<BYTE, std::function<NBTBase *(void)>> cons;
	};

	template<class T>
	TAG_Compound_ptr_t Read(T& t)
	{
		return std::dynamic_pointer_cast<TAG_Compound>(NBTHelper::Instance().Read(t));
	}

// # ===========================================================================

	template<BYTE ID, typename T1, typename T2>
	template<typename I>
	void NBTArray<ID, T1, T2>::init(const char *s, I i1, I i2)
	{
		NBTBase::setName(s);

		delete[] values;

		length = 0;
		for(I i = i1 ; i != i2 ; ++i) if(!++length) assert(false);

		T2 *t = values = length ? new T2[length] : NULL;
		while(i1 != i2)
		{
			*t++ = *i1++;
		}
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	template<typename I>
	void NBTList<ID>::init(const char *s, I i1, I i2)
	{
		NBTBase::setName(s);

		delete[] tags;

		length = 0;
		for(I i = i1 ; i != i2 ; ++i) if(!++length) assert(false);

		if(length) tagIds = (*i1)->getID();

		NBT_ptr_t *t = tags = length ? new NBT_ptr_t[length] : NULL;
		while(i1 != i2)
		{
			assert((*i1)->getID() == tagIds);
			*t++ = *i1++;
		}
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	template<typename I>
	void NBTTagCompound<ID>::init(const char *s, I i1, I i2)
	{
		NBTBase::setName(s);

		tags.clear();

		while(i1 != i2)
		{
			const char *s = (*i1)->NBTBase::getName();

			assert(s&&*s);

			tags[std::string(s)] = *i1++;
		}
	}

// # ===========================================================================

#ifndef NBT_IMP
	extern template class NBTSimple<1, BYTE>; 
	extern template class NBTSimple<2, WORD>;
	extern template class NBTSimple<3, DWORD>;
	extern template class NBTSimple<4, QWORD>;
	extern template class NBTSimple<5, float>;
	extern template class NBTSimple<6, double>;
	extern template class NBTArray<7, DWORD, BYTE>;
	extern template class NBTString<8>;
	extern template class NBTList<9>;
	extern template class NBTTagCompound<10>;
	extern template class NBTArray<11, DWORD, DWORD>;
#endif

// # ===========================================================================
}

#endif

