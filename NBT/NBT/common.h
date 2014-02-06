#ifndef NBT_COMMON_H
#define NBT_COMMON_H

#include <cstdint>
#include <memory>

namespace NBT
{
	typedef std::uint8_t   BYTE;
	typedef std::uint16_t  WORD;
	typedef std::uint32_t DWORD;
	typedef std::uint64_t QWORD;

	typedef WORD  SHORT;
	typedef DWORD   INT;
	typedef QWORD  LONG;

// # ---------------------------------------------------------------------------

	class NBTBase;
	template<BYTE, typename> class NBTSimple;
	template<BYTE, typename, typename> class NBTArray;
	template<BYTE> class NBTString;
	template<BYTE> class NBTList;
	template<BYTE> class NBTTagCompound;

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

	typedef std::shared_ptr<NBTBase> NBT_ptr_t;

	typedef std::shared_ptr<TAG_Byte>		TAG_Byte_ptr_t;
	typedef std::shared_ptr<TAG_Short>		TAG_Short_ptr_t;
	typedef std::shared_ptr<TAG_Int>		TAG_Int_ptr_t;
	typedef std::shared_ptr<TAG_Long>		TAG_Long_ptr_t;
	typedef std::shared_ptr<TAG_Float>		TAG_Float_ptr_t;
	typedef std::shared_ptr<TAG_Double>		TAG_Double_ptr_t;
	typedef std::shared_ptr<TAG_Byte_Array>	TAG_Byte_Array_ptr_t;
	typedef std::shared_ptr<TAG_String>		TAG_String_ptr_t;
	typedef std::shared_ptr<TAG_List>		TAG_List_ptr_t;
	typedef std::shared_ptr<TAG_Compound>	TAG_Compound_ptr_t;
	typedef std::shared_ptr<TAG_Int_Array>	TAG_Int_Array_ptr_t;

// # ---------------------------------------------------------------------------

	template<typename T>
	struct TypeIDs;

	template<> struct TypeIDs<BYTE>
	{
		static const BYTE ID = 1;
	};

	template<> struct TypeIDs<WORD>
	{
		static const BYTE ID = 2;
	};

	template<> struct TypeIDs<DWORD>
	{
		static const BYTE ID = 3;
	};

	template<> struct TypeIDs<QWORD>
	{
		static const BYTE ID = 4;
	};

	template<> struct TypeIDs<float>
	{
		static const BYTE ID = 5;
	};

	template<> struct TypeIDs<double>
	{
		static const BYTE ID = 6;
	};
}

#endif

