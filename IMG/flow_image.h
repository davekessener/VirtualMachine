#ifndef FLOW_IMAGE_H
#define FLOW_IMAGE_H

#include <iostream>
#include <fstream>
#include <assert.h>
#include "basic_image.h"

namespace IMG
{
	template<typename T>
	class flow_image : public IMG::basic_image<T>
	{
		public:
			flow_image(const std::string&, const basic_image<T>&);
			flow_image(const std::string&, int);
			~flow_image( );
			void draw(const basic_image<T>&);
			void flush( );
			void close( );
		private:
			std::string fn;
			std::fstream file;
			bitmap_header_t bmh;
			DIB_t dib;
	};

#include "flow_image.cpp"
}

#endif

