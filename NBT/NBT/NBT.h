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
			const char *getName(void) { return reinterpret_cast<const char *>(name); }
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
			BYTE getID(void)
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
			NBTSimple(T t) { init("", t); }
			NBTSimple(const char *s = "", T t = T()) { init(s, t); }
			T get(void) { return value; }
			void set(T t) { value = t; }
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			void init(const char *, T);
			T value;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	class NBTArray : public _NBTBase<ID>, public std::vector<T2>
	{
		typedef std::vector<T2> vec_t;
		public:
			NBTArray(const char *s = "") { NBTBase::setName(s); }
			template<typename T>
			NBTArray(const char *s, T t1, T t2) { init(s, t1, t2); }
			NBTArray(const char *s, std::initializer_list<T2> v) { init(s, v); }
			template<typename T>
			NBTArray(T t1, T t2) { init("", t1, t2); }
			NBTArray(std::initializer_list<T2> v) { init("", v); }
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			template<typename T>
			void init(const char *, T, T);
			void init(const char *, std::initializer_list<T2>);
	};

	template<BYTE ID>
	class NBTString : public NBTArray<ID, WORD, BYTE>
	{
		public:
			NBTString(const char *s, const char *v) : 
				NBTArray<ID, WORD, BYTE>(
						s, 
						reinterpret_cast<const BYTE *>(v),
						reinterpret_cast<const BYTE *>(v ? v + strlen(v) : NULL)) { }
			NBTString(const char *s = "") : NBTArray<ID, WORD, BYTE>(s) { }
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTList : public _NBTBase<ID>, public std::vector<NBT_ptr_t>
	{
		typedef std::vector<NBT_ptr_t> vec_t;
		public:
			NBTList(const char *s = "", BYTE id = 0) : tagIds(id) { NBTBase::setName(s); }
			template<typename T>
			NBTList(const char *s, T t1, T t2) : tagIds(0) { init(s, t1, t2); }
			NBTList(const char *s, std::initializer_list<NBT_ptr_t> v) : tagIds(0) { init(s, v); }
			NBTList(BYTE id) : tagIds(id) { NBTBase::setName(""); }
			template<typename T>
			NBTList(T t1, T t2) : tagIds(0) { init("", t1, t2); }
			NBTList(std::initializer_list<NBT_ptr_t> v) : tagIds(0) { init("", v); }
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			template<typename T>
			void init(const char *, T, T);
			void init(const char *, std::initializer_list<NBT_ptr_t>);
			BYTE tagIds;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTTagCompound : public _NBTBase<ID>, protected std::map<std::string, NBT_ptr_t>
	{
		typedef std::map<std::string, NBT_ptr_t> map_t;
		public:
			NBTTagCompound(const char *s = "") { NBTBase::setName(s); }
			template<typename T>
			NBTTagCompound(const char *s, T t1, T t2) { init(s, t1, t2); }
			NBTTagCompound(const char *s, std::initializer_list<NBT_ptr_t> v) { init(s, v); }
			template<typename T>
			NBTTagCompound(T t1, T t2) { init("", t1, t2); }
			NBTTagCompound(std::initializer_list<NBT_ptr_t> v) { init("", v); }
//			bool hasTag(const std::string&);
//			NBT_ptr_t getTag(const std::string&);
//			template<typename T>
//			std::shared_ptr<T> getTag(const std::string&);
//			template<typename T>
//			void setTag(
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			void init(const char *);
			template<typename T>
			void init(const char *, T, T);
			void init(const char *, std::initializer_list<NBT_ptr_t>);
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

		vec_t::clear();

		DWORD length = 0;
		for(I i = i1 ; i != i2 ; ++i) if(!++length) assert(false);

		while(i1 != i2)
		{
			vec_t::push_back(*i1++);
		}
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	template<typename I>
	void NBTList<ID>::init(const char *s, I i1, I i2)
	{
		NBTBase::setName(s);

		vec_t::clear();

		DWORD length = 0;
		for(I i = i1 ; i != i2 ; ++i) if(!++length) assert(false);

		if(length) tagIds = (*i1)->getID();

		while(i1 != i2)
		{
			assert((*i1)->getID() == tagIds);
			vec_t::push_back(*i1++);
		}
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	template<typename I>
	void NBTTagCompound<ID>::init(const char *s, I i1, I i2)
	{
		NBTBase::setName(s);

		map_t::clear();

		while(i1 != i2)
		{
			const char *s = (*i1)->NBTBase::getName();

			assert(s&&*s);

			map_t::operator[](std::string(s)) = *i1++;
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

