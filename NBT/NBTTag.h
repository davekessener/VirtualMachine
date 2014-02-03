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
#include "gzstream.h"
#include "NBT.h"
#include "NBTReader.h"
#include "NBTWriter.h"

namespace NBT
{
	class NBTBase
	{
		public:
			NBTBase(void) : name(NULL) { std::cerr << "!!! Base constructor" << std::endl; }
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
			virtual BYTE getID( ) = 0;
		protected:
			void setName(const char *);
			virtual void _write(const nbtostream&) = 0;
			virtual void _read(const nbtistream&) = 0;
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
			inline NBTArray(T1 l, T2 *v = NULL) : values(NULL) { init(NULL, l, v); }
			inline NBTArray(const char *s = "", T1 l = 0, const T2 *v = NULL) : values(NULL) { init(s, l, v); }
			inline NBTArray(std::initializer_list<T2> v) : values(NULL) { init("", v); }
			inline NBTArray(const char *s, std::initializer_list<T2> v) : values(NULL) { init(s, v); }
			inline ~NBTArray(void)
			{
				delete[] values;
			}
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
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
				delete[] tags;
			}
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
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
				delete[] tags;
			}
		protected:
			void _write(const nbtostream&);
			void _read(const nbtistream&);
		private:
			void init(const char *, int, NBT_ptr_t *);
			void init(const char *, std::initializer_list<NBT_ptr_t>);
			int length;
			NBT_ptr_t *tags;
	};

// # ---------------------------------------------------------------------------

	class NBTHelper
	{
		public:
			NBT_ptr_t Read(std::istream&);
			NBT_ptr_t Read(gzip::igzstream&);
			NBTBase *Default(BYTE);
			NBTBase *Read(const nbtistream&);
			static NBTHelper& instance( );
		private:
			NBTHelper( );
			~NBTHelper( );
			NBTHelper(const NBTHelper&);
			NBTHelper& operator=(const NBTHelper&);

			std::map<BYTE, std::function<NBTBase *(void)>> cons;
	};

// # ===========================================================================

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
}

#endif

