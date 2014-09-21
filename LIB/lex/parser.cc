#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>
//#define DEBUG_LOG
#include "lex.h"
//#undef DEBUG_LOG
#include "Tokenizer.hpp"
#include <dav/Logger.h>

//using namespace dav;
//using namespace dav::lex;

using dav::Translation;
using dav::String;
using dav::lex::Parser;
using dav::lex::Hook;
using dav::lex::Literal;
using dav::lex::ID;
using dav::lex::Empty;
using dav::lex::End;
using dav::MakeTypeList;
	
typedef String<'P', 'A', 'R', 'S', 'E', 'R'> PARSER_S;
typedef String<'V', 'A', 'R'> VARD_S;
typedef String<'B', 'E', 'G', 'I', 'N'> BEGIN_S;
typedef String<'E', 'N', 'D'> END_S;
typedef String<'R', 'U', 'L', 'E'> RULE_S;
typedef String<'='> ASSIGN_S;
typedef String<'E', 'M', 'P', 'T', 'Y'> EMPTY_S;
typedef String<'F', 'I', 'N', 'A', 'L'> FINAL_S;
typedef String<'H', 'O', 'O', 'K'> HOOK_S;
typedef String<'H', 'O', 'O', 'K', '_', 'R', 'A', 'W'> HOOK_RAW_S;
typedef String<'I', 'D'> ID_S;
typedef String<'P', 'R', 'I', 'N', 'T'> PRINT_S;
typedef String<';'> REND_S;
typedef String<'R'> R_S;
typedef String<'*'> AST_S;
typedef String<'$'> VAR_S;
typedef String<'('> OPARA_S;
typedef String<')'> CPARA_S;

typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '_', ']', '[', 'a', '-', 'z', 'A', '-', 'z', '0', '-', '9', '_', ']', '*'> ID_NAME_REGEX;
typedef String<'"', '(', '.', '|', '\\', 'n', ')', '*', '"'> ID_ST_REGEX;

// # ===========================================================================

template<typename T> using vec = std::vector<T>;
template<typename T> using ptr = std::shared_ptr<T>;

struct Rules;

class Elem
{
	public:
		virtual ~Elem( ) noexcept { }
		inline std::string eval(Rules& rules) const { return i_doEval(rules); }
	private:
		virtual std::string i_doEval(Rules& rules) const = 0;
};
typedef ptr<Elem> Elem_ptr;

class Rule
{
	typedef vec<Elem_ptr> Elem_vec;
	typedef vec<Elem_vec> vec_t;

	public:
		Rule(const std::string& name) : name_(name), elems_(1) { }
		inline void addElem(Elem_ptr p) { elems_.back().push_back(p); }
		inline void next( ) { if(!elems_.back().empty()) elems_.push_back(Elem_vec()); }
		inline const std::string& name( ) const { return name_; }
		inline vec_t& each( ) { return elems_; }
	private:
		std::string name_;
		vec_t elems_;
};
typedef ptr<Rule> Rule_ptr;

class Rules : public vec<Rule_ptr>
{
	public:
		inline Rules( ) : vec<Rule_ptr>(), final_(false) { }
		inline void finalize( )
			{ assert(!final_); int i = 0; for(const auto& r : *this) ids_[r->name()] = i++; final_ = true; }
		inline int getID(const std::string& name) const { assert(final_); if(!ids_.count(name)) throw std::string("ERR: rule '" + name + "' is not known!"); return ids_.at(name); }
	private:
		std::map<std::string, int> ids_;
		bool final_;
};

class ElemID : public Elem
{
	public:
		explicit inline ElemID(const std::string& c, bool l = false) : Elem(), c_(c), l_(l) { }
	private:
		inline std::string i_doEval(Rules& rules) const
			{ return (l_ ? "Literal<" : "ID<") + c_ + ">"; }
	private:
		std::string c_;
		bool l_;
};

class ElemHook : public Elem
{
	public:
		inline ElemHook(int id, const std::string& c, bool raw = false)
			: Elem(), id_(lexical_cast<std::string>(id)), c_(c), raw_(raw) { }
		inline const std::string& content( ) const { return c_; }
		inline bool raw( ) const { return raw_; }
		inline std::string name( ) const { return "CustomHook" + id_; }
	private:
		inline std::string i_doEval(Rules& rules) const
			{ return "Hook<" + name() + ">"; }
	private:
		std::string id_, c_;
		bool raw_;
};

class ElemTranslation : public Elem
{
	public:
		inline explicit ElemTranslation(const std::string& name) : Elem(), name_(name) { }
	private:
		inline std::string i_doEval(Rules& rules) const
			{ return "Translation<" + lexical_cast<std::string>(rules.getID(name_)) + ">"; }
	private:
		std::string name_;
};

class ElemStatic : public Elem
{
	public:
		inline explicit ElemStatic(const std::string& s) : Elem(), s_(s) { }
	private:
		inline std::string i_doEval(Rules& rules) const { return s_; }
	private:
		std::string s_;
};

class ElemPrint : public Elem
{
	public:
		inline explicit ElemPrint(const std::string& s) : Elem(), s_(s) { }
	private:
		inline std::string i_doEval(Rules& rules) const { return "Print<" + s_ + ">"; }
	private:
		std::string s_;
};

class Data
{
	public:
		inline void setParserName(const std::string& s) { assert(name_.empty()); name_ = s; }
		void finalize(std::ostream&);
		inline void selectRule(const std::string& s) { addRule(); curRule_.reset(new Rule(s)); }
		inline void nextRule( ) { assert(static_cast<bool>(curRule_)); curRule_->next(); }
		inline void selectVar(const std::string& s) { curVar_ = s; LOG("Selected variable '%s'", s.data()); }
		inline void assignVar(const std::string& s) { assert(!curVar_.empty()); vars_[curVar_] = s; LOG("Set current var '%s' to '%s'", curVar_.data(), s.data()); }
		inline void pollVar(const std::string& s) { assert(!s.empty()&&vars_.count(s)); strbuf_ << vars_.at(s); }
		inline void addString(const std::string& s) { strbuf_ << s.substr(1, s.length() - 2); }
		inline void commit( ) { input_ = strbuf_.str(); strbuf_.clear(); strbuf_.str(std::string()); }
		inline void addStatic(const std::string& s) { addElement(Elem_ptr(new ElemStatic(s))); }
		inline void addID(const std::string& s) { std::string t; addElement(Elem_ptr(new ElemID(t = createString(s), literal_))); LOG("Added %s('%s' => '%s')", (literal_ ? "Literal" : "ID"), s.data(), t.data()); }
		inline void setLiteral(bool f) { literal_ = f; }
		inline void addPrint(const std::string& s) { std::string t; addElement(Elem_ptr(new ElemPrint(t = createString(s)))); LOG("Added Print('%s' => '%s')", s.data(), t.data()); }
		inline void setHook(bool f) { raw_ = !f; }
		inline void addHook(const std::string& s) { Elem_ptr p(new ElemHook(hids_++, s, raw_)); addElement(p); hooks_.push_back(p); }
		inline void addTranslation(const std::string& s) { addElement(Elem_ptr(new ElemTranslation(s))); }
		inline const std::string& getBuffer( ) const { return input_; }
	public:
		static Data& Instance( ) { static Data d; return d; }
	private:
		inline Data( ) : literal_(true), raw_(false), hids_(0) { }
		~Data( ) = default;
		std::string createString(const std::string& str)
		{
			int i = 0;
			for(const std::string& s : strings_)
			{
				if(s == str) break;
				++i;
			}
			if(i == (long)strings_.size()) strings_.push_back(str);
			return "Var" + lexical_cast<std::string>(i);
		}
		void addElement(Elem_ptr p) { assert(static_cast<bool>(curRule_)); curRule_->addElem(p); }
		void addRule( ) { if(static_cast<bool>(curRule_)) rules_.push_back(curRule_); }
	private:
		std::string name_;
		Rules rules_;
		Rule_ptr curRule_;
		std::stringstream strbuf_;
		std::string input_;
		vec<Elem_ptr> hooks_;
		std::map<std::string, std::string> vars_;
		vec<std::string> strings_;
		std::string curVar_;
		bool literal_, raw_;
		int hids_;
	private:
		Data(const Data&) = delete;
		Data& operator=(const Data&) = delete;
};

void Data::finalize(std::ostream& os)
{
	std::string ucname(name_); 
	std::transform(ucname.cbegin(), ucname.cend(), ucname.begin(),
		[](const char& c) { return c >= 'a' && c <= 'z' ? c + 'A' - 'a' : c; });

	std::string lcname(name_); 
	std::transform(lcname.cbegin(), lcname.cend(), lcname.begin(),
		[](const char& c) { return c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c; });

	addRule();
	rules_.finalize();
	
	// inclock
	os << "#ifndef DAV_PARSER_" << ucname << "_H\n"
	      "#define DAV_PARSER_" << ucname << "_H\n\n"

	// headers
		  "#include <iostream>\n"
		  "#include <vector>\n"
		  "#include <iterator>\n"
	      "#include \"lex.h\"\n"
	      "#include \"Tokenizer.hpp\"\n\n"

	// namespace
	      "namespace dav { namespace " << lcname << " {\n";

	// strings
	{
		int i = 0;
		for(const auto& s : strings_)
		{
			os << "typedef String<";

			bool first = true;
			for(const char& c : s)
			{
				if(!first) os << ", ";
				os << '\'';
				switch(c)
				{
					case '\n':
						os << "\\n";
						break;
					case '\t':
						os << "\\t";
						break;
					case '\\':
					case '\'':
						os << '\\';
					default:
						os << c;
				}
				os << '\'';
				first = false;
			}

			os << "> Var" << lexical_cast<std::string>(i++) << ";\n";
		}
		os << "\n";
	}

	// hooks
	for(const auto& p : hooks_)
	{
		ElemHook &hook(*dynamic_cast<ElemHook *>(&*p));

		if(!hook.raw())
		{
			os << "template<typename SymTable, typename Output>\n"
			      "struct " << hook.name() << "\n{\n"
			      "\tstatic void run(SymTable& symtable, Output& output)\n\t{\n";
		}

		os << hook.content() << "\n";

		if(!hook.raw())
		{
			os << "\t}\n};\n";
		}

		os << "\n";
	}

	// parser
	os << "typedef lex::Parser\n<\n"
	      "\tMakeTypeList\n"
	      "\t<\n";
	
	bool first = true;
	for(const auto& rule : rules_)
	{
		if(!first) os << ",\n";

		os << "\t\tMakeTypeList\n"
		      "\t\t<\n";

		bool primera = true;
		for(const auto& v : rule->each())
		{
			if(!primera) os << ",\n";

			os << "\t\t\tMakeTypeList<";

			bool ichiban = true;
			for(const auto& p : v)
			{
				if(!ichiban) os << ", ";

				os << p->eval(rules_);

				ichiban = false;
			}
			assert(!ichiban);

			os << ">";
			primera = false;
		}
		assert(!primera);

		os << "\n\t\t>";
		first = false;
	}
	assert(!first);
	
	os << "\n\t>\n"
	      ">\n"
	   << name_ << "Parser;\n\n"

	// namespace end
	      "} }\n\n"

	// main
	      "int main(int argc, char *argv[])\n{\n"
		  "\tusing dav::Printer;\n"
		  "\tusing dav::Tokenizer;\n"
		  "\tusing dav::" << lcname << "::" << name_ << "Parser;\n\n"
		  "\ttypedef std::vector<std::string> svec;\n"
		  "\ttypedef std::istream_iterator<std::string> iiter;\n\n"
	      "\tsvec args(argv, argv + argc);\n"
	      "\tsvec input;\n\n"
		  "\t{\n"
		  "\t\tstd::stringstream ss;\n"
		  "\t\tstd::for_each(iiter(std::cin), iiter(), [&ss](const std::string& s) { ss << s << \" \"; });\n"
		  "\t\tstd::string s(ss.str());\n"
		  "\t\tinput.resize(s.size());\n"
		  "\t\tstd::copy(s.cbegin(), s.cend(), input.begin());\n"
		  "\t}\n\n"
		  "\tPrinter printer;\n\n"
		  "\tTokenizer::parse(input.cbegin(), input.cend(), printer);\n"
		  "\t" << name_ << "Parser::parse(printer.cbegin(), printer.cend(), std::cout);\n\n"
		  "\tstd::cout << std::endl;\n\n"
		  "\treturn 0;\n"
	      "}\n\n"

	// endif
	      "#endif\n";
}

// # ===========================================================================

#define MXT_HOOK_BEGIN(name) \
template<typename ST, typename O> \
struct name \
{ \
	static void run(ST& st, O& o)

#define MXT_HOOK_END \
};

#define MXT_CURTOK st.at("*")

MXT_HOOK_BEGIN(SetName)
{
	Data::Instance().setParserName(MXT_CURTOK);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(Finalize)
{
	Data::Instance().finalize(o);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(SetRule)
{
	Data::Instance().selectRule(MXT_CURTOK);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushRule)
{
	Data::Instance().nextRule();
}
MXT_HOOK_END

MXT_HOOK_BEGIN(SetVariable)
{
	Data::Instance().selectVar(MXT_CURTOK);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushVariable)
{
	Data::Instance().assignVar(Data::Instance().getBuffer());
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PollVariable)
{
	Data::Instance().pollVar(MXT_CURTOK);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(CommitStringBuffer)
{
	Data::Instance().commit();
}
MXT_HOOK_END

MXT_HOOK_BEGIN(AddString)
{
	Data::Instance().addString(MXT_CURTOK);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushEmpty)
{
	Data::Instance().addStatic("Empty");
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushFinal)
{
	Data::Instance().addStatic("End");
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushLiteral)
{
	Data::Instance().setLiteral(true);
	Data::Instance().addID(Data::Instance().getBuffer());
}
MXT_HOOK_END

MXT_HOOK_BEGIN(SetRegex)
{
	Data::Instance().setLiteral(false);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(SetLiteral)
{
	Data::Instance().setLiteral(true);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushAnyID)
{
	Data::Instance().addID("\".*\"");
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushID)
{
	Data::Instance().addID(Data::Instance().getBuffer());
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushPrintID)
{
	Data::Instance().addStatic("PrintID");
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushPrint)
{
	Data::Instance().addPrint(Data::Instance().getBuffer());
}
MXT_HOOK_END

MXT_HOOK_BEGIN(SetHook)
{
	Data::Instance().setHook(true);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(SetRaw)
{
	Data::Instance().setHook(false);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushHook)
{
	Data::Instance().addHook(Data::Instance().getBuffer());
}
MXT_HOOK_END

MXT_HOOK_BEGIN(PushTranslation)
{
	Data::Instance().addTranslation(MXT_CURTOK);
}
MXT_HOOK_END

MXT_HOOK_BEGIN(Error)
{
	throw std::string("WTF!?!");
}
MXT_HOOK_END

#undef MXT_HOOK_END
#undef MXT_HOOK_BEGIN

typedef Parser
<
	MakeTypeList
	<
		MakeTypeList
		<
			MakeTypeList<Translation<1>, End>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<PARSER_S>, ID<ID_NAME_REGEX>, Hook<SetName>, Translation<17>, Translation<2>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<BEGIN_S>, Translation<3>, Hook<Finalize>>
		>,
		MakeTypeList // 3
		<
			MakeTypeList<Literal<END_S>>,
			MakeTypeList<Translation<4>>
		>,
		MakeTypeList // 4
		<
			MakeTypeList<Literal<RULE_S>, ID<ID_NAME_REGEX>, Hook<SetRule>, Translation<5>>
		>,
		MakeTypeList // 5
		<
			MakeTypeList<Translation<6>, Translation<5>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList // 6
		<
			MakeTypeList<Literal<ASSIGN_S>, Hook<PushRule>, Translation<7>>
		>,
		MakeTypeList // 7
		<
			MakeTypeList<Translation<9>, Translation<8>>
		>,
		MakeTypeList // 8
		<
			MakeTypeList<Translation<9>, Translation<8>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList // 9
		<
			MakeTypeList<Translation<3>>,
			MakeTypeList<Literal<EMPTY_S>, Hook<PushEmpty>>,
			MakeTypeList<Literal<FINAL_S>, Hook<PushFinal>>,
			MakeTypeList<Literal<ID_S>, Translation<10>>,
			MakeTypeList<Literal<PRINT_S>, Translation<13>>,
			MakeTypeList<Translation<15>>,
			MakeTypeList<Translation<18>, Hook<PushLiteral>>,
			MakeTypeList<ID<ID_NAME_REGEX>, Hook<PushTranslation>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<OPARA_S>, Translation<11>, Literal<CPARA_S>>,
			MakeTypeList<Empty, Hook<PushAnyID>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<R_S>, Hook<SetRegex>, Translation<12>>,
			MakeTypeList<Empty, Hook<SetLiteral>, Translation<12>>
		>,
		MakeTypeList
		<
			MakeTypeList<Translation<18>, Hook<PushID>>,
			MakeTypeList<Empty, Hook<PushAnyID>>
		>,
		MakeTypeList // 13
		<
			MakeTypeList<Literal<OPARA_S>, Translation<14>, Literal<CPARA_S>>,
			MakeTypeList<Empty, Hook<PushPrintID>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<AST_S>, Hook<PushPrintID>>,
			MakeTypeList<Translation<18>, Hook<PushPrint>>,
			MakeTypeList<Empty, Hook<PushPrintID>>
		>,
		MakeTypeList // 15
		<
			MakeTypeList<Literal<HOOK_S>, Hook<SetHook>, Translation<16>>,
			MakeTypeList<Literal<HOOK_RAW_S>, Hook<SetRaw>, Translation<16>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<OPARA_S>, Translation<18>, Hook<PushHook>, Literal<CPARA_S>>
		>,
		MakeTypeList // 17
		<
			MakeTypeList<Literal<VARD_S>, ID<ID_NAME_REGEX>, Hook<SetVariable>, Literal<ASSIGN_S>, Translation<18>, Hook<PushVariable>, Translation<17>>,
			MakeTypeList<Empty>
		>,
		MakeTypeList // 18
		<
			MakeTypeList<Translation<19>>
		>,
		MakeTypeList
		<
			MakeTypeList<Literal<VAR_S>, ID<ID_NAME_REGEX>, Hook<PollVariable>, Translation<20>>,
			MakeTypeList<ID<ID_ST_REGEX>, Hook<AddString>, Translation<20>>
		>,
		MakeTypeList
		<
			MakeTypeList<Translation<19>>,
			MakeTypeList<Empty, Hook<CommitStringBuffer>>
		>
	>
>
PGen;

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	std::stringstream ss;

	while(!std::cin.eof())
	{
		std::string s("");

		std::getline(std::cin, s);

		ss << s << " ";
	}

	std::string s(ss.str());
	std::vector<std::string> v(s.size());
	std::copy(s.cbegin(), s.cend(), v.begin());

	dav::Printer p;

	dav::Tokenizer::parse(v.cbegin(), v.cend(), p);

//	std::copy(p.cbegin(), p.cend(), std::ostream_iterator<std::string>(std::cout, "\n"));

	PGen::parse(p.cbegin(), p.cend(), std::cout);

	return 0;
}
catch(const std::string& e)
{
	std::cerr << e << std::endl;
	return 1;
}

