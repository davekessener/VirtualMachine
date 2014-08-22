#ifndef NBTMAIN_H
#define NBTMAIN_H

#include <iostream>
#include <initializer_list>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <dav/gzstream.h>
#include "common.h"
#include "Reader.h"
#include "Writer.h"
#include "NBTException.h"

namespace nbt
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
			void write(std::ostream&) const;
			void write(dav::gzip::ogzstream&) const;
  			void write(const nbtostream&) const;
			void read(std::istream&);
			void read(dav::gzip::igzstream&);
			void read(const nbtistream&);
			void setName(const std::string&);
			std::string getName(void) { return std::string(reinterpret_cast<const char *>(name)); }
			virtual BYTE getID( ) const = 0;
		protected:
			virtual void _write(const nbtostream&) const = 0;
			virtual void _read(const nbtistream&) = 0;
			BYTE *name;

			template<BYTE> friend class NBTList;
	};

// # ---------------------------------------------------------------------------

	template<BYTE id>
	class _NBTBase : public NBTBase
	{
		public:
			BYTE getID(void) const
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
			NBTSimple(const std::string& s = "", T t = T()) { init(s, t); }
			T get(void) { return value; }
			void set(T t) { value = t; }
		protected:
			void _write(const nbtostream&) const;
			void _read(const nbtistream&);
		private:
			void init(const std::string& , T);
			T value;
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	class NBTArray : public _NBTBase<ID>, public std::vector<T2>
	{
		typedef std::vector<T2> vec_t;
		public:
			typedef std::shared_ptr<NBTArray<ID, T1, T2>> ptr_t;

			NBTArray(const std::string& s = "") { NBTBase::setName(s); }
			template<template<typename T, typename = std::allocator<T>> class C>
			NBTArray(const std::string& s, C<NBT_ptr_t>& c) { init(s, c.begin(), c.end()); }
			template<typename T>
			NBTArray(const std::string& s, T t1, T t2) { init(s, t1, t2); }
			NBTArray(const std::string& s, std::initializer_list<T2> v) { init(s, v); }
			template<template<typename T, typename = std::allocator<T>> class C>
			NBTArray(C<NBT_ptr_t>& c) { init("", c.begin(), c.end()); }
			template<typename T>
			NBTArray(T t1, T t2) { init("", t1, t2); }
			NBTArray(std::initializer_list<T2> v) { init("", v); }
			std::vector<T2> get() { return std::vector<T2>(vec_t::begin(), vec_t::end()); }
		protected:
			void _write(const nbtostream&) const;
			void _read(const nbtistream&);
		private:
			template<typename T>
			void init(const std::string& , T, T);
			void init(const std::string& , std::initializer_list<T2>);
	};

	template<BYTE ID>
	class NBTString : public NBTArray<ID, WORD, BYTE>
	{
		typedef std::vector<BYTE> vec_t;
		public:
			typedef std::shared_ptr<NBTString<ID>> ptr_t;

			NBTString(const std::string& s, const std::string& v) : 
				NBTArray<ID, WORD, BYTE>(
						s, 
						reinterpret_cast<const BYTE *>(v.c_str()),
						reinterpret_cast<const BYTE *>(v.c_str() + v.length())) { }
			NBTString(const std::string& s = "") : NBTArray<ID, WORD, BYTE>(s) { }
			std::string get() { return std::string(vec_t::begin(), vec_t::end()); }
	};

// # ---------------------------------------------------------------------------

	template<BYTE ID>
	class NBTList : public _NBTBase<ID>, protected std::vector<NBT_ptr_t>
	{
		typedef std::vector<NBT_ptr_t> vec_t;
		public:
			typedef std::shared_ptr<NBTList<ID>> ptr_t;

			template<typename T>
			class iterator
			{
				typedef std::vector<NBT_ptr_t>::iterator i_t;

				public:
					std::shared_ptr<T> operator*() { return std::dynamic_pointer_cast<T>(*i); }
					T& operator->() { return *operator*(); }
					iterator<T>& operator++() { ++i; return *this; }
					bool operator==(const iterator& _i) { return i == _i.i; }
					bool operator!=(const iterator& _i) { return i != _i.i; }
				private:
					friend class NBTList<ID>;
					iterator(i_t _i) : i(_i) { }
					i_t i;
			};
			template<typename T>
				iterator<T> begin(void) { return iterator<T>(vec_t::begin()); }
			vec_t::iterator begin(void) { return vec_t::begin(); }
			template<typename T>
				iterator<T> end(void) { return iterator<T>(vec_t::end()); }
			vec_t::iterator end(void) { return vec_t::end(); }
			NBTList(const std::string& s = "", BYTE id = 0) : tagIds(id) { NBTBase::setName(s); }
			template<template<typename T, typename = std::allocator<T>> class C>
				NBTList(const std::string& s, C<NBT_ptr_t>& c) : tagIds(0) { init(s, c.begin(), c.end()); }
			template<typename T>
				NBTList(const std::string& s, T t1, T t2) : tagIds(0) { init(s, t1, t2); }
			NBTList(const std::string& s, std::initializer_list<NBT_ptr_t> v) : tagIds(0) { init(s, v); }
			NBTList(BYTE id) : tagIds(id) { NBTBase::setName(""); }
			template<template<typename T, typename = std::allocator<T>> class C>
				NBTList(C<NBT_ptr_t>& c) : tagIds(0) { init("", c.begin(), c.end()); }
			template<typename T>
				NBTList(T t1, T t2) : tagIds(0) { init("", t1, t2); }
			NBTList(std::initializer_list<NBT_ptr_t> v) : tagIds(0) { init("", v); }
			int size( ) { return vec_t::size(); }
			int tagID( ) { return tagIds; }
			template<typename T>
				std::shared_ptr<T> tagAt(int i) { return std::dynamic_pointer_cast<T>(tagAt(i)); }
			NBT_ptr_t tagAt(int i) { assert(i>=0&&i<vec_t::size()); return vec_t::at(i); }
			template<typename T>
				void addTag(std::shared_ptr<T> tag)
					{ assert(!tagIds||tagIds==tag->getID()); vec_t::push_back(std::dynamic_pointer_cast<NBTBase>(tag)); if(!tagIds) tagIds = tag->getID(); }
			void addTag(NBT_ptr_t tag) { assert(!tagIds||tagIds==tag->getID()); vec_t::push_back(tag); if(!tagIds) tagIds = tag->getID(); }
			void removeTag(NBT_ptr_t tag) { vec_t::erase(std::find(begin(), end(), tag)); }
			void removeTag(vec_t::iterator i) { vec_t::erase(i); }
			template<typename T>
				void removeTag(iterator<T> i) { vec_t::erase(i.i); }
		protected:
			void _write(const nbtostream&) const;
			void _read(const nbtistream&);
		private:
			template<typename T>
				void init(const std::string& , T, T);
			void init(const std::string& , std::initializer_list<NBT_ptr_t>);
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
			NBTTagCompound(const std::string& s = "") : Names(*this), Tags(*this) { NBTBase::setName(s); }
			template<typename T>
				NBTTagCompound(const std::string& s, T t1, T t2) : Names(*this), Tags(*this) { init(s, t1, t2); }
			template<template<typename T, typename = std::allocator<T>> class C>
				NBTTagCompound(const std::string& s, C<NBT_ptr_t> c) : Names(*this), Tags(*this) { init(s, c.begin(), c.end()); }
			NBTTagCompound(const std::string& s, std::initializer_list<NBT_ptr_t> v) : Names(*this), Tags(*this) { init(s, v); }
			template<typename T>
				NBTTagCompound(T t1, T t2) : Names(*this), Tags(*this) { init("", t1, t2); }
			template<template<typename T, typename = std::allocator<T>> class C>
				NBTTagCompound(C<NBT_ptr_t>& c) : Names(*this), Tags(*this) { init("", c.begin(), c.end()); }
			NBTTagCompound(std::initializer_list<NBT_ptr_t> v) : Names(*this), Tags(*this) { init("", v); }
			bool hasTag(const std::string& s) { return map_t::count(s) > 0; }
			bool empty( ) { return map_t::empty(); }
			size_t size( ) { return map_t::size(); }
			NBT_ptr_t getTag(const std::string& s) { return hasTag(s) ? map_t::operator[](s) : NBT_ptr_t(NULL); }
			template<typename T>
				std::shared_ptr<T> getTag(const std::string& s) { return std::dynamic_pointer_cast<T>(getTag(s)); }
			void setTag(const std::string& s, NBT_ptr_t p) { p->setName(s); map_t::operator[](s) = p; }
			void setTag(NBT_ptr_t p) { map_t::operator[](p->getName()) = p; }
			template<typename T>
				void setTag(const std::string& name, std::shared_ptr<T> p)
					{ p->setName(name); map_t::operator[](name) = std::dynamic_pointer_cast<NBTBase>(p); }
			template<typename T>
				void setTag(std::shared_ptr<T> p)
					{ map_t::operator[](p->getName()) = std::dynamic_pointer_cast<NBTBase>(p); }
			bool removeTag(const std::string& s) { return hasTag(s) ? map_t::erase(s) : false; }
			template<typename T>
				void set(const std::string& name, T t)
					{ map_t::operator[](name) = NBT_ptr_t(new NBTSimple<BasicTypeIDs<T>::ID, T>(name, t)); }
			void setByte(const std::string& s, BYTE v) { set<BYTE>(s, v); }
			void setShort(const std::string& s, WORD v) { set<WORD>(s, v); }
			void setInt(const std::string& s, DWORD v) { set<DWORD>(s, v); }
			void setLong(const std::string& s, QWORD v) { set<QWORD>(s, v); }
			void setFloat(const std::string& s, float v) { set<float>(s, v); }
			void setDouble(const std::string& s, double v) { set<double>(s, v); }
			void setString(const std::string& name, const std::string& s) { setTag(NBT_ptr_t(new TAG_String(name, s))); }
			void setByteArray(const std::string&s, BYTE *v, int n) { assert(n>0&&v); setByteArray<BYTE *>(s, v, v + n); }
			template<template<typename T, typename = std::allocator<T>> class C>
				void setByteArray(const std::string&s, C<BYTE> c) { setByteArray<typename C<BYTE>::iterator>(s, c.begin(), c.end()); }
			template<typename I>
				void setByteArray(const std::string& s, I i1, I i2) { setTag(NBT_ptr_t(new TAG_Byte_Array(s, i1, i2))); }
			void setIntArray(const std::string&s, DWORD *v, int n) { assert(n>0&&v); setIntArray<DWORD *>(s, v, v + n); }
			template<template<typename T, typename = std::allocator<T>> class C>
				void setIntArray(const std::string&s, C<DWORD> c) { setIntArray<typename C<DWORD>::iterator>(s, c.begin(), c.end()); }
			template<typename I>
				void setIntArray(const std::string& s, I i1, I i2) { setTag(NBT_ptr_t(new TAG_Int_Array(s, i1, i2))); }
			void setTagList(const std::string& s, TAG_List::ptr_t p) { setTag<TAG_List>(s, p); }
			void setTagList(TAG_List::ptr_t p) { setTag<TAG_List>(p); }
			template<template<typename T, typename = std::allocator<T>> class C>
				void setTagList(const std::string& s, C<NBT_ptr_t> c) { setTagList(s, c.begin(), c.end()); }
			template<typename I>
				void setTagList(const std::string& s, I i1, I i2) { setTag(NBT_ptr_t(new TAG_List(s, i1, i2))); }
			void setCompoundTag(const std::string& s, TAG_Compound_ptr_t p) { setTag<TAG_Compound>(s, p); }
			void setCompoundTag(TAG_Compound_ptr_t p) { setTag<TAG_Compound>(p); }
			template<template<typename T, typename = std::allocator<T>> class C>
				void setCompoundTag(const std::string& s, C<NBT_ptr_t> c) { setCompoundTag(s, c.begin(), c.end()); }
			template<typename I>
				void setCompoundTag(const std::string& s, I i1, I i2) { setTag(NBT_ptr_t(new TAG_Compound(s, i1, i2))); }
			BYTE getByte(const std::string& s) { return getTag<TAG_Byte>(s)->get(); }
			WORD getShort(const std::string& s) { return getTag<TAG_Short>(s)->get(); }
			DWORD getInt(const std::string& s) { return getTag<TAG_Int>(s)->get(); }
			QWORD getLong(const std::string& s) { return getTag<TAG_Long>(s)->get(); }
			float getFloat(const std::string& s) { return getTag<TAG_Float>(s)->get(); }
			double getDouble(const std::string& s) { return getTag<TAG_Double>(s)->get(); }
			std::vector<BYTE> getByteArray(const std::string& s) { return getTag<TAG_Byte_Array>(s)->get(); }
			std::string getString(const std::string& s) { return getTag<TAG_String>(s)->get(); }
			TAG_List::ptr_t getTagList(const std::string& s) { return getTag<TAG_List>(s); }
			ptr_t getCompoundTag(const std::string& s) { return getTag<TAG_Compound>(s); }
			std::vector<DWORD> getIntArray(const std::string& s) { return std::dynamic_pointer_cast<TAG_Int_Array>(map_t::operator[](s))->get(); }
		protected:
			void _write(const nbtostream&) const;
			void _read(const nbtistream&);
		private:
			void init(const std::string&);
			void init(const std::string&, std::initializer_list<NBT_ptr_t>);
			template<typename T> void init(const std::string&, T, T);
	};

// # ---------------------------------------------------------------------------

	class NBTHelper
	{
		public:
			NBT_ptr_t Read(std::istream&);
			NBT_ptr_t Read(dav::gzip::igzstream&);
			NBTBase *Default(BYTE, const std::string& = "");
			NBTBase *Read(const nbtistream&);
			static NBTHelper& Instance( );
		private:
			NBTHelper( );
			~NBTHelper( );
			NBTHelper(const NBTHelper&);
			NBTHelper& operator=(const NBTHelper&);

			std::map<BYTE, std::function<NBTBase *(const std::string&)>> cons;
	};

// # ===========================================================================

	template<class T>
	TAG_Compound::ptr_t Read(T& t)
	{
		return std::dynamic_pointer_cast<TAG_Compound>(NBTHelper::Instance().Read(t));
	}

	template<class T, typename ... A>
	typename T::ptr_t Make(A ... a)
	{
		return typename T::ptr_t(new T(a...));
	}

	inline NBT_ptr_t Make(BYTE id, const std::string& name = "")
	{
		return NBT_ptr_t(NBTHelper::Instance().Default(id, name));
	}

// # ---------------------------------------------------------------------------

	template<BYTE ID, typename T1, typename T2>
	template<typename I>
	void NBTArray<ID, T1, T2>::init(const std::string& s, I i1, I i2)
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
	void NBTList<ID>::init(const std::string& s, I i1, I i2)
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
	void NBTTagCompound<ID>::init(const std::string& name, I i1, I i2)
	{
		NBTBase::setName(name);

		map_t::clear();

		while(i1 != i2)
		{
			std::string s = (*i1)->NBTBase::getName();

			assert(s.length());

			map_t::operator[](s) = *i1++;
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

