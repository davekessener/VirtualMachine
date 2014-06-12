#ifndef CURSES_H
#define CURSES_H

#include <cstdint>

namespace ncurses
{
	namespace Keys
	{
		enum
		{
			ENTER = '\n',
			LEFT = 0x80000001,
			RIGHT,
			UP,
			DOWN,
			BACKSPACE,
			ESCAPE
		};
	}

	struct Attributes
	{
		static const int UNDERSCORE;
	};

	enum class Borders : std::uint8_t
	{
		SINGLE,
		DOUBLE,
		BOLD_SINGLE
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
}

#endif

