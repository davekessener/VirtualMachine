#ifndef NBTEXCEPTION_H
#define NBTEXCEPTION_H

#include <exception>
#include <string>

#ifdef NBTEXCEPTION_MAIN
#endif

namespace nbt
{
	class NBTException : public std::exception
	{
		public:
			NBTException(const std::string&);
			virtual ~NBTException( ) throw( );
			virtual const char *what( ) const throw( );
		private:
			std::string msg;
	};
}

#endif

