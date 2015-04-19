#ifndef DAV_DEA_READER_H
#define DAV_DEA_READER_H

#include <memory>
#include "DEA.h"

namespace AF
{
	namespace DEA
	{
		class Reader
		{
			public:
				static DEA Generate(std::istream& in) { return Instance().generate(in); }
			private:
				static Reader& Instance( ) { static Reader r; return r; }
			private:
				DEA generate(std::istream&);
			private:
				struct Impl;
				std::unique_ptr<Impl> impl_;
			private:
				Reader( );
				~Reader( );
				Reader(const Reader&) = delete;
				Reader& operator=(const Reader&) = delete;
		};
	}
}

#endif

