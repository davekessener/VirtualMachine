#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>

#ifdef NULL
#undef NULL
#endif

#define NULL nullptr
#define NULL_t decltype(nullptr)

namespace display
{
	namespace Keys
	{
		enum
		{
			ERR = -1,
			ENTER = '\n',
			TAB = '\t',
			LEFT = 0x80000001,
			RIGHT,
			UP,
			DOWN,
			BACKSPACE,
			ESCAPE
		};
	}

	enum class Borders : std::uint8_t
	{
		SINGLE,
		DOUBLE,
		BOLD_SINGLE,
		ASCII
	};

	template<Borders b>
	struct BorderTypes;

	template<>
	struct BorderTypes<Borders::SINGLE>
	{
		static const wchar_t SIDES_VERTICAL = L'\u2502';
		static const wchar_t SIDES_HORIZONTAL = L'\u2500';
		static const wchar_t CORNER_TOPLEFT = L'\u250c';
		static const wchar_t CORNER_TOPRIGHT = L'\u2510';
		static const wchar_t CORNER_BOTTOMLEFT = L'\u2514';
		static const wchar_t CORNER_BOTTOMRIGHT = L'\u2518';
	};

	template<>
	struct BorderTypes<Borders::DOUBLE>
	{
		static const wchar_t SIDES_VERTICAL = L'\u2551';
		static const wchar_t SIDES_HORIZONTAL = L'\u2550';
		static const wchar_t CORNER_TOPLEFT = L'\u2554';
		static const wchar_t CORNER_TOPRIGHT = L'\u2557';
		static const wchar_t CORNER_BOTTOMLEFT = L'\u255a';
		static const wchar_t CORNER_BOTTOMRIGHT = L'\u255d';
	};

	template<>
	struct BorderTypes<Borders::BOLD_SINGLE>
	{
		static const wchar_t SIDES_VERTICAL = L'\u2503';
		static const wchar_t SIDES_HORIZONTAL = L'\u2501';
		static const wchar_t CORNER_TOPLEFT = L'\u250f';
		static const wchar_t CORNER_TOPRIGHT = L'\u2513';
		static const wchar_t CORNER_BOTTOMLEFT = L'\u2517';
		static const wchar_t CORNER_BOTTOMRIGHT = L'\u251b';
	};

	template<>
	struct BorderTypes<Borders::ASCII>
	{
		static const char SIDES_VERTICAL = '|';
		static const char SIDES_HORIZONTAL = '-';
		static const char CORNER_TOPLEFT = '+';
		static const char CORNER_TOPRIGHT = '+';
		static const char CORNER_BOTTOMLEFT = '+';
		static const char CORNER_BOTTOMRIGHT = '+';
	};
}

#endif

