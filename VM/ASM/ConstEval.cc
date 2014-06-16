#include <sstream>
#include <memory>
#include <stack>
#include <cassert>
#include "ConstEval.h"
#include "SymTable.h"
#include "Line.h"
#include "stringtools.h"
#include "Logger.h"

namespace vm { namespace assembler { namespace Evaluator {

class Extractor
{
	public:
		Extractor(const std::string& s)
		{
			Line l(s, "", 0);

			for(const Token& t : l)
			{
				out_.push(t.str());
			}
			while(!out_.empty())
			{
				in_.push(out_.top());
				out_.pop();
			}
		}
		inline const std::string top( ) { return in_.empty() ? "" : in_.top(); }
		inline void next( ) { if(!in_.empty()) { out_.push(in_.top()); in_.pop(); 
			//LOG("> Nexted: '%s' -> '%s'", out_.top().c_str(), in_.empty() ? "" : in_.top().c_str()); 
			} }
		inline void back( ) { if(!out_.empty()) { in_.push(out_.top()); out_.pop(); 
			//LOG("> Backed: '%s' -> '%s'", in_.top().c_str(), out_.empty() ? "" : out_.top().c_str()); 
			} }
	private:
		std::stack<std::string> in_, out_;
};

typedef std::shared_ptr<Extractor> ex_t;

ex_t to_eval;
SymTable *symtbl;
int offset;

// eExpr(O0) := O1 *( '&&,||' O1 )
//		 O1  := O2 *( '==,!=,<,<=,>,>=' O2 )
//		 O2  := O3 *( '<<,>>' O3 )
//		 O3  := O4 *(  '+,-'  O4 )
//		 O4  := O5 *(  '*,/'  O5 )
//		 O5  := O6 *( '&,|,^' O6 )
//		 O6  := *( '~,-,!' )  O7
//		 O7  := C | '(' O0 ')'

WORD O0( );
WORD O1( );
WORD O2( );
WORD O3( );
WORD O4( );
WORD O5( );
WORD O6( );
WORD O7( );

WORD consteval(const std::string& line, SymTable& tbl, int pos)
{
	to_eval.reset(new Extractor(line));
	symtbl = &tbl;
	offset = pos;

//	LOG("Evaluating const-expr '%s' [...]", line.c_str());

	return O0();
}

WORD O0(void)
{
	//LOG("> Entering O0 with '%s'", to_eval->top().c_str());
	WORD t = O1();
	std::string s(to_eval->top());

	while(s == "&&" || s == "||")
	{
		to_eval->next();

		if(s == "&&")
		{
			t = t && O1();
		}
		else if(s == "||")
		{
			t = t || O1();
		}

		s = to_eval->top();
	}

	//LOG("> Leaving O0 width '%s'", to_eval->top().c_str());

	return t;
}

WORD O1(void)
{
	//LOG("> Entering O1 with '%s'", to_eval->top().c_str());
	WORD t = O2();
	std::string s(to_eval->top());

	while(s == "==" || s == "!=" || s == "<" || s == "<=" || s == ">" || s == ">=")
	{
		to_eval->next();

		if(s == "==")
		{
			t = t == O2();
		}
		else if(s == "!=")
		{
			t = t != O2();
		}
		else if(s == "<")
		{
			t = t < O2();
		}
		else if(s == "<=")
		{
			t = t <= O2();
		}
		else if(s == ">")
		{
			t = t > O2();
		}
		else if(s == ">=")
		{
			t = t >= O2();
		}

		s = to_eval->top();
	}

	//LOG("> Leaving O1 width '%s'", to_eval->top().c_str());

	return t;
}

WORD O2(void)
{
	//LOG("> Entering O2 with '%s'", to_eval->top().c_str());
	WORD t = O3();
	std::string s(to_eval->top());

	while(s == "<<" || s == ">>")
	{
		to_eval->next();

		if(s == "<<")
		{
			t <<= O3();
		}
		else if(s == ">>")
		{
			t >>= O3();
		}

		s = to_eval->top();
	}

	//LOG("> Leaving O2 width '%s'", to_eval->top().c_str());

	return t;
}

WORD O3(void)
{
	//LOG("> Entering O3 with '%s'", to_eval->top().c_str());
	WORD t = O4();
	std::string s(to_eval->top());

	while(s == "+" || s == "-")
	{
		to_eval->next();

		if(s == "+")
		{
			t += O4();
		}
		else if(s == "-")
		{
			t -= O4();
		}

		s = to_eval->top();
	}

	//LOG("> Leaving O3 width '%s'", to_eval->top().c_str());

	return t;
}

WORD O4(void)
{
	//LOG("> Entering O4 with '%s'", to_eval->top().c_str());
	WORD t = O5();
	std::string s(to_eval->top());

	while(s == "*" || s == "/")
	{
		to_eval->next();

		if(s == "*")
		{
			t *= O5();
		}
		else if(s == "/")
		{
			t /= O5();
		}

		s = to_eval->top();
	}

	//LOG("> Leaving O4 width '%s'", to_eval->top().c_str());

	return t;
}

WORD O5(void)
{
	//LOG("> Entering O5 with '%s'", to_eval->top().c_str());
	WORD t = O6();
	std::string s(to_eval->top());

	while(s == "&" || s == "|" || s == "^")
	{
		to_eval->next();

		if(s == "&")
		{
			t &= O6();
		}
		else if(s == "|")
		{
			t |= O6();
		}
		else if(s == "^")
		{
			t ^= O6();
		}

		s = to_eval->top();
	}

	//LOG("> Leaving O5 width '%s'", to_eval->top().c_str());

	return t;
}

WORD O6(void)
{
	//LOG("> Entering O6 with '%s'", to_eval->top().c_str());
	std::string s(to_eval->top());

	WORD r = 0;

	if(s == "-" || s == "~" || s == "!")
	{
		to_eval->next();

		if(s == "-")
		{
			r = -O6();
		}
		else if(s == "~")
		{
			r = ~O6();
		}
		else if(s == "!")
		{
			r = !O6();
		}
	}
	else
	{
		r = O7();
	}

	//LOG("> Leaving O6 width '%s'", to_eval->top().c_str());

	return r;
}

WORD O7(void)
{
	//LOG("> Entering O7 with '%s'", to_eval->top().c_str());
	std::string s(to_eval->top());
	to_eval->next();

	WORD r = 0;

	if(s == "(")
	{
		r = O0();

		if(to_eval->top() != ")")
			throw std::string("ERR: Malformed(missing ')') const-expr");
		
		to_eval->next();
	}
	else
	{
		assert(!s.empty());
		if(s == "$")
		{
			r = offset;
		}
		else if(symtbl->knows(s))
		{
			r = symtbl->get(s);
		}
		else
		{
			std::istringstream iss(s);
			
			int i = -1;
			iss >> i;

			if(i < 0) 
				throw string_format("ERR: Malformed('%s' is not a constant!) const-expr", 
					s.c_str());

			r = static_cast<WORD>(i);
		}
	}

	//LOG("> Leaving O7 width '%s'", to_eval->top().c_str());

	return r;
}

}}}

