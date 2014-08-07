#ifndef HEADER_H
#define HEADER_H

#include <iosfwd>
#include <string>

class Header
{
	public:
		Header(std::istream&);
		Header(const Header&);
		Header& operator=(const Header&);
		~Header( ) noexcept;
		void operator()(const std::string&);
		void swap(Header&) noexcept;
	private:
		struct Impl;
		Impl *impl_;
};

#endif

