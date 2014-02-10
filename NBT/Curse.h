#ifndef CURSE_H
#define CURSE_H

#include <cstdint>
#include <map>

namespace ncurses
{
	enum class CharBufferMode : std::uint8_t
	{
		COOKED,
		CBREAK,
		RAW
	};

	enum class BufferAttribute : std::uint8_t
	{
		ECHO,
		DELAY,
		SHOW_CURSOR
	};

	enum class SetMode : std::uint8_t
	{
		SET,
		RESET,
		TOGGLE
	};

	class Curse
	{
		class _curse_config
		{
			typedef std::map<BufferAttribute, bool> AttrMap;
			typedef std::pair<BufferAttribute, bool> Attr;
			public:
				~_curse_config( );
				_curse_config(const _curse_config&);
				_curse_config& operator=(const _curse_config&);
				bool getAttr(BufferAttribute) const;
				CharBufferMode getBufferMode( ) const;
				void setAttr(BufferAttribute, bool);
				void setBufferMode(CharBufferMode);
			private:
				friend class Curse;
				_curse_config( );
				_curse_config(CharBufferMode, AttrMap&);
				AttrMap attr;
				CharBufferMode bmode;
		};
		struct _size
		{
			const size_t width, height;
			inline _size(int _width, int _height) : width(_width), height(_height) { }
			inline _size(const _size& s) : width(s.width), height(s.height) { }
		};
		struct _pos
		{
			const size_t x, y;
			inline _pos(int _x, int _y) : x(_x), y(_y) { }
			inline _pos(const _pos& p) : x(p.x), y(p.y) { }
		};
		struct _rect
		{
			const _size size;
			const _pos pos;
			inline _rect(const _size& _s, const _pos& _p) : size(_s), pos(_p) { }
			inline _rect(const _rect& r) : size(r.size), pos(r.pos) { }
		};

		public:
			typedef _curse_config Configuration;
			typedef _pos Position;
			typedef _size Size;
			typedef _rect Rect;

			void begin( );
			void end( );
			void setBufferMode(CharBufferMode);
			void setBufferAttribute(BufferAttribute, SetMode = SetMode::SET);
			Size getScreenSize( ) const;
			Position getCursorPosition( ) const;
			void moveCursor(int, int);
			void putch(int);
			void print(const char *);
			void printw(const wchar_t *);
			void printf(const char *, ...);
			void printfw(const wchar_t *, ...);
			wchar_t getch( );
			int getline(char *, int = -1);
			int getlinew(wchar_t *, int = -1);
			CharBufferMode getBufferMode( );
			bool isBufferAttributeSet(BufferAttribute);
			Configuration backupConfiguration( );
			void restoreConfiguration( );
			void restoreConfiguration(const Configuration&);
			static Curse& instance( );
		private:
			Curse( );
			Curse(const Curse&);
			Curse(Curse&&);
			~Curse( );
			Curse& operator=(const Curse&);
			Curse& operator=(Curse&&);
			bool isRunning;
			CharBufferMode buffermode;
			Configuration::AttrMap attributes;
			Configuration config;
	};
}

#endif

