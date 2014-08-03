#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include "inc.h"

class Image
{
	public:
		Image( ) : loaded_(false) /*id_(0)*/ { }
		void load(const std::string&);
		void create( );
//		void bind( ) const;
		void erase( );
		void draw(int, int, int = 0, int = 0) const;
		void center(int, int) const;
		inline bool loaded( ) const { return loaded_; }
		inline int width( ) const { return width_; }
		inline int height( ) const { return height_; }
//		static DWORD docreate(const BYTE *, int, int, int * = nullptr, int * = nullptr);
	private:
//		DWORD id_;
		int width_, height_;
//		int pw_, ph_;
		bool loaded_;
		std::vector<BYTE> data_;
};

#endif

