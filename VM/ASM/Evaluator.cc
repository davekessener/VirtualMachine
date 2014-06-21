#include <sstream>
#include <cassert>
#include "Evaluator.h"
#include "Line.h"
#include "Logger.h"
#include "ASMException.h"
#include "SymTable.h"
#include "ConstEval.h"

namespace vm { namespace assembler { namespace Evaluator {

namespace
{
	eval_t eval_const(const std::string&, const SymTable&, int);
	eval_t eval_reg(const std::string&);
	eval_t eval_mem(const std::string&, const SymTable&, int);
}

eval_t eval(Parameter p, const std::string& v, const SymTable& sym, int pos)
{
	if(p!=deduceParameter(v)) MXT_LOGANDTHROW("ERR: Detected parameter type mismatch for '%s'"
		"(supposedly %d)", v.c_str(), static_cast<int>(p));
	
	switch(p)
	{
		case Parameter::CONSTANT:
			return eval_const(v, sym, pos);
		case Parameter::REGISTER:
			return eval_reg(v);
		case Parameter::MEMORY:
			return eval_mem(v, sym, pos);
		default:
			MXT_LOGANDTHROW("ERR: unknown paramter type '%d' for argument '%s'", p, v.c_str());
			break;
	}
}

Parameter deduceParameter(const std::string& e)
{
	auto error = [&e](const char *s, char c)
		{
			MXT_LOGANDTHROW("ERR(%s): Malformed parameter @'%c'(%d) '%s'", s, c, static_cast<int>(c), e.c_str());
		};

	std::istringstream iss(e);
	char c = '\0';

	iss >> c;

	switch(c)
	{
		case '\0':
			MXT_LOGANDTHROW("ERR: Cannot deduce parameter type from empty string.");
			break;
		case 'r':
		{
			bool n = true;

			while(true)
			{
				c = '\0';
				iss >> c;
				if(!c) break;
				if(c < '0' || c > '9') error("malformed register: unexpected char", c);
				n = false;
			}

			if(n) error("malformed register: missing reg. number", c);

//			LOG("Parameter '%s' has type %c", e.c_str(), Parameter::REGISTER);

			return Parameter::REGISTER;
		}
		break;
		case '[':
		{
			c = '\0'; iss >> c;
			if(c != 'r') error("malformed memory: 'r' missing", c);

			bool n = true;

			while(true)
			{
				c = '\0';
				iss >> c;
				if(c == ']')
				{
					c = '\0';
					iss >> c;
					n = false;
					if(!c) break;
					else error("malformed memory: trailing characters", c);
				}
				if(!c) break;
//				if(c < '0' || c > '9') error(2, c);
//				n = false;
			}

			if(n) error("malformed memory: not properly terminated", c);

//			LOG("Parameter '%s' has type %c", e.c_str(), Parameter::MEMORY);

			return Parameter::MEMORY;
		}
		break;
		default:
		{
//			LOG("Parameter '%s' has type %c", e.c_str(), Parameter::CONSTANT);

			return Parameter::CONSTANT;
		}
		break;
	}

	assert(!"Should never be here!");
}

namespace
{
	eval_t eval_const(const std::string& l, const SymTable& s, int p)
	{
		try
		{
			return eval_t(1, consteval(l, s, p));
		}
		catch(const std::string& msg)
		{
			MXT_LOGANDTHROW("%s [in '%s']", msg.c_str(), l.c_str());
		}
	}

	eval_t eval_reg(const std::string& expr)
	{
		auto error = [&expr]
			{
				MXT_LOGANDTHROW("ERR: Expected register, got '%s'", expr.c_str());
			};

		if(expr.size() < 2 || expr[0] != 'r') error();

		std::string e(expr.substr(1));

		for(char c : e)
		{
			if(c < '0' || c > '9') error();
		}

		std::istringstream iss(e);
		WORD r;
		iss >> r;

		return eval_t(1, r);
	}

	eval_t eval_mem(const std::string& expr, const SymTable& sym, int pos)
	{
		auto error = [&expr](int i, char c)
			{
				MXT_LOGANDTHROW("ERR(%d): Expected memory, got '%c'(%d) ['%s']", 
					i, c, static_cast<int>(c), expr.c_str());
			};

		std::istringstream iss(expr);
		int offset = 1;
		char c = '\0';

		iss >> c; if(c != '[') error(0, c);
		iss >> c; if(c != 'r') error(1, c);
		
		std::stringstream ss;
		while(true)
		{
			c = '\0';
			iss >> c;
			switch(c)
			{
				case '-':
					offset = -1;
				case '+':
				{
					std::string t;
					std::getline(iss, t);
					if(t.back() != ']') error(5, t.back());
					t.pop_back();
					offset *= consteval(t, sym, pos);
					c = '\0';
					break;
				}
				case ']':
					offset = 0;
					c = '\0';
					iss >> c;
					if(c) error(4, c);
					break;
			}
			if(!c) break;
			if(c < '0' || c > '9') error(2, c);
			ss << c;
		}

		if(ss.str().empty()) error(3, c);

		WORD r;
		ss >> r;

//		LOG("> Read register '%d'!", r);

		eval_t ret;
		ret.push_back(r);
		ret.push_back(static_cast<WORD>(offset));

		return ret;
	}
}

}}}

