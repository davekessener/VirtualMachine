#ifndef PKMN_EDITOR_TEXT_H
#define PKMN_EDITOR_TEXT_H

#include "../common.h"

namespace editor
{
	class Text
	{
		public:
			static void drawText(int x, int y, const std::string& s, DWORD c = 0)
				{ instance().doDrawText(x, y, s, c); }
			static const int C_W = 8;
		private:
			Text( );
			void doDrawText(int, int, const std::string&, DWORD = 0) const;
			void doDrawChar(int, int, char, DWORD = 0) const;
		private:
			DWORD id_;
		private:
			static Text& instance( ) { static Text t; return t; }
		private:
			Text(const Text&) = delete;
			Text& operator=(const Text&) = delete;
	};
}

#endif

