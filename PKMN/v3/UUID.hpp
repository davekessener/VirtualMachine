#ifndef DAV_UUID_H
#define DAV_UUID_H

#include <string>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <aux>
#include "common.h"

namespace dav
{
	class UUID
	{
		enum { N = sizeof(boost::uuids::uuid) };
		public:
			UUID( )
			{
				boost::uuids::uuid id = boost::uuids::random_generator()();

				const BYTE *p = reinterpret_cast<const BYTE *>(&id);
				for(uint i = 0 ; i < N ; ++i) data_[i] = *p++;
			}
			UUID(const std::string& s)
			{
				if(s.size() != 2 * N) throw std::string(dav::aux::stringify("ERR: Invalid length of UUID! Expected ", N, ", received ", s.size(), "!"));

				const char *p = s.data();
				auto gt = [](char c) -> uint { return c >= '0' && c <= '9' ? c - '0' : (c >= 'a' && c <= 'f' ? c - 'a' + 10 : c - 'A' + 10); };
				for(uint i = 0 ; i < N ; ++i, p += 2)
				{
					data_[i] = (gt(p[0]) << 4) | gt(p[1]);
				}
			}
			bool operator==(const UUID& id) const
				{ for(uint i = 0 ; i < N ; ++i) if(data_[i] != id.data_[i]) return false; return true; }
			bool operator!=(const UUID& id) const { return !(*this == id); }
			std::string toString( ) const
			{
				std::stringstream ss;

				for(uint i = 0 ; i < N ; ++i)
				{
					ss << dav::aux::to_hex(data_[i]);
				}

				return ss.str();
			}
			static UUID Nil( )
			{
				std::string s("");
				for(uint i = 0 ; i < N ; ++i) s += "00";
				UUID id(s);
				return id;
			}
			template<typename T> static T rand( ) { return dav::aux::generateUID<T>(); }
			static bool Bool(double d) { return rand<uint>() / (static_cast<uint>(-1) + 1.0) < d; }
		private:
			BYTE data_[N];
	};
}

#endif

