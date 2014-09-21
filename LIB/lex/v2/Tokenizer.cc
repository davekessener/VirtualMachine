#include "Tokenizer.h"
#include "lex.hpp"

namespace dav
{
	namespace tokenizer
	{
		using lex::Analysis;
		using lex::Production;
		using lex::Literal;
		using lex::Empty;
		using lex::End;
		using lex::Hook;
		using lex::Match;

		typedef String<'[', '0', '-', '9', ']'> ID_DIGIT_REGEX;
		typedef String<'[', '0', '-', '9', ']'> ID_DIGIT_REGEX;
		typedef String<'+'> ADD_S;
		typedef String<'-'> SUB_S;
		typedef String<'\''> SQUOTE_S;
		typedef String<'"'> QUOTE_S;
		typedef String<'.'> DOT_S;
		typedef String<'n'> N_S;
		typedef String<'t'> T_S;
		typedef String<'\n'> NL_S;
		typedef String<'\t'> TAB_S;
		typedef String<'\\'> BACKSLASH_S;
		typedef String<'[', 'e', 'E', ']'> ID_EXP_REGEX;
		typedef String<'.'> ID_ANY_REGEX;
		
		typedef String<'[', '\\', 't', ' ', ']'> ID_WS_REGEX;
		typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '_', ']'> ID_IDT_BEG_REGEX;
		typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', '_', ']'> ID_IDT_END_REGEX;
		typedef String<'[', '\\', '`', '\\', '~', '\\', '!', '\\', '@', '\\', '#', '\\', '$', '\\', '%', '\\', '^', '\\', '&', '\\', '*', '\\', '(', '\\', ')', '\\', '-', '\\', '_', '\\', '=', '\\', '+', '\\', '[', '\\', ']', '\\', '{', '\\', '}', '\\', '\\', '\\', '|', '\\', '/', '\\', '?', '\\', '.', '\\', '>', '\\', ',', '\\', '<', '\\', ':', '\\', ';', ']'> ID_OP_REGEX;

		class TokenBuffer
		{
			public:
				explicit TokenBuffer(Writer& w) : write_(w) { inst() = this; }
				~TokenBuffer( ) { if(inst() == this) inst() = nullptr; }
				void add(const std::string& s) { buf_ << s; }
				void push( ) { write_.push(buf_.str()); buf_.clear(); buf_.str(std::string()); }
			private:
				Writer &write_;
				std::ostringstream buf_;
			public:
				static TokenBuffer& buf( ) { assert(inst()); return *inst(); }
			private:
				static TokenBuffer *& inst( ) { static TokenBuffer *tb; return tb; }
		};

		template<typename S = NIL>
		struct Push
		{
			static_assert(HasName<S>::value, "'S' has to have static 'toString'!");

			static void run(void)
			{
				TokenBuffer::buf().add(S::toString());
			}
		};

		template<>
		struct Push<NIL>
		{
			static void run(void)
			{
				TokenBuffer::buf().add(lex::StringStore::get());
			}
		};

		struct Next
		{
			static void run(void)
			{
				TokenBuffer::buf().push();
			}
		};

		typedef Analysis
		<
			MakeTypeList
			<
				MakeTypeList // 0
				<
					MakeTypeList<Production<1>, End>
				>,
				MakeTypeList // 1
				<
					MakeTypeList<Production<2>, Production<1>>,
					MakeTypeList<Empty>
				>,
				MakeTypeList // 2
				<
					MakeTypeList<Match<ID_WS_REGEX>>,
					MakeTypeList<Production<3>, Hook<Next>>
				>,
				MakeTypeList // 3
				<
					MakeTypeList<Production< 4>>,
					MakeTypeList<Production< 6>>,
					MakeTypeList<Production<13>>,
					MakeTypeList<Production<12>>
				>,
				MakeTypeList // 4
				<
					MakeTypeList<Match<ID_IDT_BEG_REGEX>, Hook<Push<>>, Production<5>>
				>,
				MakeTypeList // 5
				<
					MakeTypeList<Match<ID_IDT_END_REGEX>, Hook<Push<>>, Production<5>>,
					MakeTypeList<Empty>
				>,
				MakeTypeList // 6
				<
					MakeTypeList<Production<8>, Production<10>, Production<11>>
				>,
				MakeTypeList // 7
				<
					MakeTypeList<Literal<ADD_S>, Production<8>>,
					MakeTypeList<Literal<SUB_S>, Hook<Push<SUB_S>>, Production<8>>,
					MakeTypeList<Production<8>>
				>,
				MakeTypeList // 8
				<
					MakeTypeList<Match<ID_DIGIT_REGEX>, Hook<Push<>>, Production<9>>
				>,
				MakeTypeList // 9
				<
					MakeTypeList<Match<ID_DIGIT_REGEX>, Hook<Push<>>, Production<9>>,
					MakeTypeList<Empty>
				>,
				MakeTypeList // 10
				<
					MakeTypeList<Literal<DOT_S>, Hook<Push<DOT_S>>, Production<8>>,
					MakeTypeList<Empty>
				>,
				MakeTypeList // 11
				<
					MakeTypeList<Match<ID_EXP_REGEX>, Hook<Push<>>, Production<7>>,
					MakeTypeList<Empty>
				>,
				MakeTypeList // 12
				<
					MakeTypeList<Match<ID_ANY_REGEX>, Hook<Push<>>>
				>,
				MakeTypeList // 13
				<
					MakeTypeList<Literal<QUOTE_S>, Hook<Push<QUOTE_S>>, Production<14>>,
					MakeTypeList<Literal<SQUOTE_S>, Hook<Push<SQUOTE_S>>, Production<15>, Literal<SQUOTE_S>, Hook<Push<SQUOTE_S>>>
				>,
				MakeTypeList // 14
				<
					MakeTypeList<Literal<QUOTE_S>, Hook<Push<QUOTE_S>>>,
					MakeTypeList<Production<15>, Production<14>>
				>,
				MakeTypeList // 15
				<
					MakeTypeList<Literal<BACKSLASH_S>, Production<16>>,
					MakeTypeList<Match<ID_ANY_REGEX>, Hook<Push<>>>
				>,
				MakeTypeList // 16
				<
					MakeTypeList<Literal<N_S>, Hook<Push<NL_S>>>,
					MakeTypeList<Literal<T_S>, Hook<Push<TAB_S>>>,
					MakeTypeList<Literal<BACKSLASH_S>, Hook<Push<BACKSLASH_S>>>,
					MakeTypeList<Literal<QUOTE_S>, Hook<Push<QUOTE_S>>>,
					MakeTypeList<Literal<SQUOTE_S>, Hook<Push<SQUOTE_S>>>
				>
			>
		>
		Tokenizer;

		void parse(Reader& in, Writer& out)
		{
			TokenBuffer tb(out);

			Tokenizer::parse(in);
		}
	}
}

