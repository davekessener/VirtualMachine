#include <sstream>
#include <deque>
#include <stack>
#include <map>
#include <algorithm>
#include <cassert>
#include "Preprocessor.h"
#include "Tokenizer.h"
#include "Logger.h"
#include "ASMException.h"
#include "SymTable.h"
#include "stringtools.h"

#define MXT_PD_INC ".inc"
#define MXT_PD_EQU ".equ"
#define MXT_PD_DB  ".db"

namespace vm { namespace assembler {

namespace
{
	std::vector<Line> insertQuoteDB(const Token&);
	Line insertDB(const std::vector<Token>&);
	std::string dbChar(const char *&);
	inline std::string dbChar(const char *&& s) { return dbChar(s); }
}

class Preprocessor::Impl
{
	private:
		void fillBuffer( );
	private:
		void process(const Line&);
		std::vector<Token> substitute(const Token&);
		Line substitute(const Line&);
	private:
		friend class Preprocessor;
		std::deque<Line> buffer_;
		std::stack<Tokenizer> toks_;
		std::vector<std::string> incs_;
		Line recent_;
		std::map<std::string, std::vector<Token>> sym_;
};

// # ===========================================================================

void Preprocessor::Impl::fillBuffer(void)
{
	while(buffer_.empty() && !toks_.empty())
	{
		while(!toks_.empty() && !toks_.top().ready()) toks_.pop();
		if(toks_.empty()) break;
		process(substitute(toks_.top().getline()));
	}
}

// # ---------------------------------------------------------------------------

template<typename T, typename TT, template<typename, typename> class C1, template<typename, typename> class C2>
inline void operator<<(C1<T, TT>& v1, const C2<T, TT>& v2)
{
	v1.insert(v1.end(), v2.cbegin(), v2.cend());
}

template<template<typename, typename> class C, typename T, typename TT>
inline void operator<<(C<T, TT>& c, const T& t)
{
	c.push_back(t);
}

template<typename T>
inline bool contains(T& c, const typename T::value_type& v)
{
	return std::find(c.begin(), c.end(), v) != c.end();
}

// # ---------------------------------------------------------------------------

void Preprocessor::Impl::process(const Line& line)
{
	if(line.empty()) return;

	LOG("[PP] Processing line '%s' [...]", line.str().c_str());

	if(line[0][0] == '.')
	{
		std::string cmd(line[0].str());
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		LOG("[PP] > Directive '%s'", cmd.c_str());

		if(cmd == MXT_PD_INC)
		{
			if(line.size() != 2) 
				MXT_LOGANDTHROW_T(line[0], "ERR: Expected 1 argument only. (Did you forget the \"\"s?");
			std::string fn(line[1].str());
			if(fn.front() != '"' || fn.back() != '"')
				MXT_LOGANDTHROW_T(line[1], "ERR: Expected a string as argument.");
			fn = fn.substr(1, fn.size() - 1);
			if(contains(incs_, fn))
			{
				LOG("[PP] Cyclic dependency with file '%s'", fn.c_str());
			}
			else
			{
				incs_.push_back(fn);
				toks_.push(Tokenizer(fn));
				LOG("[PP] Included file '%s'", fn.c_str());
			}
		}
		else if(cmd == MXT_PD_EQU)
		{
			if(line.size() < 3) MXT_LOGANDTHROW_T(line[0], "ERR: Not enough arguments for pp-directive '.equ'");
			
			std::string id(line[1].str());
			std::vector<Token> v(line.cbegin() + 2, line.cend());
			
			std::string sv;
			std::for_each(v.begin(), v.end(), [&sv](const Token& t) { sv += t.str(); });

			if(sym_.count(id))
					MXT_LOGANDTHROW_T(line[1], "ERR: Redefining '.equ' '%s'.", id.c_str());
			
			sym_[id] = v;
			
			LOG("[PP] Mapped '%s' -> '%s'", id.c_str(), sv.c_str());
		}
		else if(cmd == MXT_PD_DB)
		{
			if(line.size() < 2) MXT_LOGANDTHROW_T(line[0], "ERR: Expected parameters.");
			std::vector<Token> tmp(line.cbegin() + 1, line.cend());
			tmp.push_back(Token(",", "", 0, 0));

			std::vector<Line> dblines;

			for(auto i = tmp.begin(), j = i ; i != tmp.end() ; ++i)
			{
				if(i->str() == ",")
				{
					try
					{
						std::vector<Token> toks(j, i);

						if(toks.size() == 1 && toks.front()[0] == '"')
						{
							std::vector<Line> _(insertQuoteDB(toks.front()));
							dblines.insert(dblines.end(), _.begin(), _.end());
//							dblines << insertQuoteDB(toks.front());
						}
						else
						{
							dblines.push_back(insertDB(toks));
//							dblines << insertDB(toks);
						}
 
 						j = i + 1;
					}
					catch(const std::string& msg)
					{
						MXT_LOGANDTHROW_T(*i, "%s", msg.c_str());
					}
				}
			}

			for(const Line& l : dblines)
			{
				LOG("Line '%s' added.", l.str().c_str());
			}

			buffer_.insert(buffer_.begin(), dblines.begin(), dblines.end());
//			buffer_ << dblines;
		}
		else
		{
			MXT_LOGANDTHROW_T(line[0], "ERR: Unknown preprocessor directive '%s'!", cmd.c_str());
		}
	}
	else
	{
		buffer_.push_back(line);
	}
}

std::vector<Token> Preprocessor::Impl::substitute(const Token& t)
{
	std::vector<Token> r;

	if(sym_.count(t.str()))
	{
		for(const Token& tt : sym_[t.str()])
		{
			if(t == tt) MXT_LOGANDTHROW_T(t, "ERR: Recursive '.equ': '%s'", t.str().c_str());

			r << substitute(Token(tt.str(), t.filename(), t.line(), t.word()));
		}
	}
	else if(t.str()[0] == '\'')
	{
		try
		{
			r.push_back(Token(dbChar(t.str().c_str() + 1), t.filename(), t.line(), t.word()));
		}
		catch(const std::string& msg)
		{
			MXT_LOGANDTHROW_T(t, "%s", msg.c_str());
		}
	}
	else
	{
		r.push_back(t);
	}

	return r;
}

Line Preprocessor::Impl::substitute(const Line& line)
{
	Line l(line.filename(), line.line());
	std::vector<Token> v;

	for(auto i = line.cbegin() ; i != line.cend() ; ++i)
	{
		v << substitute(*i);
	}

	for(const Token& t : v)
	{
		l += t;
	}

	return l;
}

// # ===========================================================================

Preprocessor::Preprocessor(void) : impl_(new Impl)
{
}

Preprocessor::~Preprocessor(void) throw()
{
	delete impl_;
}

void Preprocessor::feed(const Tokenizer& token)
{
	impl_->incs_.push_back(token.filename());
	impl_->toks_.push(token);
}

Line Preprocessor::get(void)
{
	impl_->fillBuffer();

	assert(!impl_->buffer_.empty());

	impl_->recent_ = impl_->buffer_.front();
	impl_->buffer_.pop_front();

	return impl_->recent_;
}

bool Preprocessor::ready(void) const
{
	impl_->fillBuffer();

	return !impl_->buffer_.empty();
}

void Preprocessor::clear(void)
{
	Preprocessor p;
	swap(p);
}

void Preprocessor::swap(Preprocessor& p) throw()
{
	Impl *i = impl_;
	impl_ = p.impl_;
	p.impl_ = i;
}

// # ===========================================================================

namespace
{
	std::string dbChar(const char *& s)
	{
		std::ostringstream oss;

		oss << static_cast<WORD>(convert_character(s));

		return oss.str();
	}
	
	std::vector<Line> insertQuoteDB(const Token& t)
	{
		Line line(t.filename(), t.line());
		std::string st(t.str().substr(1, t.str().size() - 2));
		const char *s = st.c_str();
	
		std::vector<Line> v;
		line += MXT_PD_DB;
	
		while(*s)
		{
			Line l(line);
			l += dbChar(s);
			v.push_back(l);
		}
	
		return v;
	}
	
	Line insertDB(const std::vector<Token>& toks)
	{
		Line l(toks.front().filename(), toks.front().line());
		l += MXT_PD_DB;
	
		for(const Token& t : toks)
		{
			if(t.str().front() == '"')
			{
				MXT_LOGANDTHROW_T(t, "ERR: Malformed(defective quote?) argument for '.db'");
			}
			else
			{
				l += t;
			}
		}
	
		return l;
	}
}

// # ===========================================================================

//void dodb(const Line& line)
//{
//	if(line.size() < 2) MXT_LOGANDTHROW_T(line[0], "ERR: Expected parameters.");
//
//	std::vector<Token> vec;
//	std::ostringstream osslog;
//
//	osslog << MXT_PD_DB << " ";
//
//	for(auto i = line.cbegin() ; ++i != line.cend() ;)
//	{
//		try
//		{
//			if(i->str() == ",")
//			{
//				if(vec.empty()) MXT_LOGANDTHROW_T(*i, "ERR: Malformed(empty) argument for '.db'");
//
//				Line l(line.filename(), line.line());
//				std::vector<std::string> v;
//
//				l += MXT_PD_DB;
//
//				for(const Token& t : vec)
//				{
//					v << substitute(t);
//				}
//
//				if(v.size() == 1 && v.front().front() == '"')
//				{
//					const char *tmp = v.front().c_str();
//
//					while(*tmp)
//					{
//						Line tl(l);
//						std::ostringstream oss;
//
//						oss << static_cast<WORD>(convert_character(tmp));
//
//						tl += oss.str();
//
//						osslog << oss.str() << ", ";
//
//						buffer_.push_back(tl);
//					}
//				}
//				else if(v.size() == 1 && v.front().front() == '\'')
//				{
//					const char *tmp = v.front().c_str();
//					std::ostringstream oss;
//					oss << static_cast<WORD>(convert_character(tmp));
//					l += oss.str();
//					osslog << oss.str() << ", ";
//					buffer_.push_back(l);
//				}
//				else
//				{
//					for(const std::string& s : v)
//					{
//						if(s.front() == '"' || s.front() == '\'')
//							MXT_LOGANDTHROW_T(*i, "ERR: Malformed(defective quote?) argument for '.db'");
//						l += s;
//						osslog << s;
//					}
//
//					osslog << ", ";
//
//					buffer_.push_back(l);
//				}
//
//				vec.clear();
//			}
//			else
//			{
//				vec.push_back(*i);
//			}
//		}
//		catch(const std::string& msg)
//		{
//			MXT_LOGANDTHROW_T(*i, "%s", msg.c_str());
//		}
//	}
//
//	std::string log(osslog.str());
//	log = log.substr(0, log.size() - 2);
//	LOG("[PP] '%s' -> '%s'", line.str().c_str(), log.c_str());
//}

}}

