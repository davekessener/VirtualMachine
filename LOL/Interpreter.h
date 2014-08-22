#ifndef LOL_INTERPRETER_H
#define LOL_INTERPRETER_H

#include <dav/inc.h>
#include <string>
#include <vector>

namespace lol
{
	class Interpreter
	{
		public:
		typedef std::vector<std::string> params_t;

		public:
			static Interpreter& instance( );
			static int run(const params_t& args) { return instance().doRun(args); }
			static std::string readLine( );
			static void write(const std::string&);
			static void writeLine(const std::string&);
		private:
			Interpreter( );
			~Interpreter( );
			int doRun(const params_t&);
			std::string input( ) const;
		private:
			struct Impl;
			Impl *impl_;
		private:
			Interpreter(const Interpreter&) = delete;
			Interpreter& operator=(const Interpreter&) = delete;
	};
}

#endif

