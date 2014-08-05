#ifndef CHARSET_H
#define CHARSET_H

#include <string>

class Charset
{
	public:
		Charset( );
		~Charset( );
		void load(const std::string&);
		void renderStringAt(const std::string&, int, int, int = 0xffffff) const;
		void renderCharAt(char, int, int, int = 0xffffff) const;
		int getStringWidth(const std::string&) const;
		int getCharHeight( ) const;
		void setCharSize(int);
	private:
		struct Impl;
		Impl *impl_;
};

#endif

