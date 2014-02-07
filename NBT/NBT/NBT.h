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
			typedef std::shared_ptr<NBTSimple<ID, T>> ptr_t;

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
			typedef std::shared_ptr<NBTArray<ID, T1, T2>> ptr_t;

			NBTArray(const char *s = "") { NBTBase::setName(s); }
			template<template<typename T, typename = std::allocator<T>> class C>
			NBTArray(const char *s, C<NBT_ptr_t>& c) { init(s, c.begin(), c.end()); }
			template<typename T>
			NBTArray(const char *s, T t1, T t2) { init(s, t1, t2); }
			NBTArray(const char *s, std::initializer_list<T2> v) { init(s, v); }
			template<template<typename T, typename = std::allocator<T>> class C>
			NBTArray(C<NBT_ptr_t>& c) { init("", c.begin(), c.end()); }
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
			typedef std::shared_ptr<NBTString<ID>> ptr_t;

			NBTString(const char *s, const char *v) : 
				NBTArray<ID, WORD, BYTE>(
						s, 
						reinterpret_cast<const BYTE *>(v),
						reinterpret_cast<const BYTE *>(v ? v + strlen(v) : NULL)) { }
			NBTString(const char *s = "") : NBTArray<ID, WORD, BYTE>(s) { }
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTList : public _NBTBase<ID>, protected std::vector<NBT_ptr_t>
	{
		typedef std::vector<NBT_ptr_t> vec_t;
		public:
			typedef std::shared_ptr<NBTList<ID>> ptr_t;

			NBTList(const char *s = "", BYTE id = 0) : tagIds(id) { NBTBase::setName(s); }
			template<template<typename T, typename = std::allocator<T>> class C>
			NBTList(const char *s, C<NBT_ptr_t>& c) :tagIds(0) { init(s, c.begin(), c.end()); }
			template<typename T>
			NBTList(const char *s, T t1, T t2) : tagIds(0) { init(s, t1, t2); }
			NBTList(const char *s, std::initializer_list<NBT_ptr_t> v) : tagIds(0) { init(s, v); }
			NBTList(BYTE id) : tagIds(id) { NBTBase::setName(""); }
			template<template<typename T, typename = std::allocator<T>> class C>
			NBTList(C<NBT_ptr_t>& c) : tagIds(0) { init("", c.begin(), c.end()); }
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
			typedef std::shared_ptr<NBTTagCompound<ID>> ptr_t;

			class Names
			{
				public:
					class iterator
					{
						public:
							std::string operator*() { return _i->first; }
							const std::string *operator->() { return &_i->first; }
							iterator& operator++() { ++_i; return *this; }
							bool operator==(const iterator& i) { return _i == i._i; }
							bool operator!=(const iterator& i) { return !operator==(i); }
						private:
							friend class Names;
							iterator(std::map<std::string, NBT_ptr_t>::iterator i) : _i(i) { }
							std::map<std::string, NBT_ptr_t>::iterator _i;
					};
					iterator begin() { return iterator(_tc.begin()); }
					iterator end() { return iterator(_tc.end()); }
				private:
					friend class NBTTagCompound<ID>;
					Names(NBTTagCompound<ID>& tc) : _tc(tc) { }
					NBTTagCompound<ID> &_tc;
			} Names;
			class Tags
			{
				public:
					class iterator
					{
						public:
							NBT_ptr_t operator*() { return _i->second; }
							NBT_ptr_t operator->() { return _i->second; }
							iterator& operator++() { ++_i; return *this; }
							bool operator==(const iterator& i) { return _i == i._i; }
							bool operator!=(const iterator& i) { return !operator==(i); }
						private:
							friend class Tags;
							iterator(std::map<std::string, NBT_ptr_t>::iterator i) : _i(i) { }
							std::map<std::string, NBT_ptr_t>::iterator _i;
					};
					iterator begin() { return iterator(_tc.begin()); }
					iterator end() { return iterator(_tc.end()); }
				private:
					friend class NBTTagCompound<ID>;
					Tags(NBTTagCompound<ID>& tc) : _tc(tc) { }
					NBTTagCompound<ID> &_tc;
			} Tags;
		public:
			NBTTagCompound(const char *s = "") : Names(*this), Tags(*this) { NBTBase::setName(s); }
			
			template<typename T>
			NBTTagCompound(const char *s, T t1, T t2) : Names(*this), Tags(*this) { init(s, t1, t2); }
			
			template<template<typename T, typename = std::allocator<T>> class C>
			NBTTagCompound(const char *s, C<NBT_ptr_t> c) : Names(*this), Tags(*this) { init(s, c.begin(), c.end()); }
			
			NBTTagCompound(const char *s, std::initializer_list<NBT_ptr_t> v) : Names(*this), Tags(*this) { init(s, v); }
			
			template<typename T>
			NBTTagCompound(T t1, T t2) : Names(*this), Tags(*this) { init("", t1, t2); }
			
			template<template<typename T, typename = std::allocator<T>> class C>
			NBTTagCompound(C<NBT_ptr_t>& c) : Names(*this), Tags(*this) { init("", c.begin(), c.end()); }
			
			NBTTagCompound(std::initializer_list<NBT_ptr_t> v) : Names(*this), Tags(*this) { init("", v); }

			bool hasTag(const std::string&);
			NBT_ptr_t getTag(const std::string&);
			template<typename T> std::shared_ptr<T> getTag(const std::string&);
			void setTag(const char *, NBT_ptr_t);
			void setTag(NBT_ptr_t);
			template<typename T> void setTag(const char *, std::shared_ptr<T>);
			template<typename T> void setTag(std::shared_ptr<T>);
			template<typename T> void set(const char *, T);
			void setString(const char *, const char *);
			void setByteArray(const char *, BYTE *, int);
			template<template<typename> class C> void setByteArray(const char *, C<BYTE>&);
			template<typename I> void setByteArray(const char *, I, I);
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			void init(const char *);
			void init(const char *, std::initializer_list<NBT_ptr_t>);
			template<typename T> void init(const char *, T, T);
	};

	template<BYTE ID>
	void NBTTagCompound<ID>::setString(const char *name, const char *val)
	{
		map_t::operator[](std::string(name)) = NBT_ptr_t(new TAG_String(name, val));
	}

	template<BYTE ID>
	template<template <typename> class C>
	void NBTTagCompound<ID>::setByteArray(const char *name, C<BYTE>& c)
	{
		map_t::operator[](std::string(name)) = NBT_ptr_t(new TAG_Byte_Array(name, c.begin(), c.end()));
	}

	template<BYTE ID>
	template<typename I>
	void NBTTagCompound<ID>::setByteArray(const char *name, I i1, I i2)
	{
		map_t::operator[](std::string(name)) = NBT_ptr_t(new TAG_Byte_Array(name, i1, i2));
	}

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

// # ===========================================================================

	template<class T>
	TAG_Compound_ptr_t Read(T& t)
	{
		return std::dynamic_pointer_cast<TAG_Compound>(NBTHelper::Instance().Read(t));
	}

	template<class T, typename ... A>
	std::shared_ptr<T> Make(A ... a)
	{
		return std::shared_ptr<T>(new T(a...));
	}

// # ---------------------------------------------------------------------------

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

	template<BYTE ID>
	template<typename T>
	std::shared_ptr<T> NBTTagCompound<ID>::getTag(const std::string& name)
	{
		if(!hasTag(name)) return std::shared_ptr<T>(NULL);

		return std::dynamic_pointer_cast<T>(map_t::operator[](name));
	}

	template<BYTE ID>
	template<typename T>
	void NBTTagCompound<ID>::setTag(const char *name, std::shared_ptr<T> p)
	{
		p->setName(name);
		map_t::operator[](std::string(name)) = std::dynamic_pointer_cast<NBTBase>(p);
	}

	template<BYTE ID>
	template<typename T>
	void NBTTagCompound<ID>::setTag(std::shared_ptr<T> p)
	{
		map_t::operator[](std::string(p->getName())) = std::dynamic_pointer_cast<NBTBase>(p);
	}

	template<BYTE ID>
	template<typename T>
	void NBTTagCompound<ID>::set(const char *name, T t)
	{
		NBT_ptr_t tag(new NBTSimple<TypeIDs<T>::ID, T>(name, t));
		setTag(tag);
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

