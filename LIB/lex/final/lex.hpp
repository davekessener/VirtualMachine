#ifndef DAV_LEX_H
#define DAV_LEX_H

#include <aux>
#include <functional>
#include "lib.hpp"

#ifdef DEBUG_LOG
#	include <dav/Logger.h>
#	define MXT_LEX_LOG(...) LOG(__VA_ARGS__)
#else
#	define MXT_LEX_LOG(...)
#endif

#ifdef STD_REGEX
#	include <regex>
#	define MXT_REGEX std
#else
#	include <boost/regex.hpp>
#	define MXT_REGEX boost
#endif

namespace dav
{
	namespace lex
	{
		typedef String<'.', '*'> ID_ALL_REGEX;

		struct StringStore
		{
			static std::string& str( ) { static std::string s; return s; }
			static void set(const std::string& s) { str() = s; }
			static const std::string& get( ) { return str(); }
		};

		struct StringQueue
		{
			typedef std::function<void(const std::string&)> out_fn;
			typedef std::vector<std::string> queue_t;

			public:
				static void push(const std::string& s) { internal().push_back(s); }
				static void flush(out_fn fn)
					{ for(const auto& s : internal()) fn(s); internal().clear(); }
			private:
				static queue_t& internal( ) { static queue_t q; return q; }
		};

// # ---------------------------------------------------------------------------

#define MXT_CHECK_STRING(S) \
	static_assert(HasName<S>::value, "'S' needs to provide a static 'toString' function!");

		struct End
		{
			template<typename I>
			static bool matches(I& input)
			{
				if(!input.empty()) 
					throw aux::stringify("trailing token #", input.pos(), " '", input.top(), "'!");

				return true;
			}
		};

		struct Empty
		{
			template<typename I>
			static bool matches(I& input)
			{
				return true;
			}
		};

		template<typename F>
		struct Hook
		{
			template<typename I>
			static bool matches(I& input)
			{
				F::run();

				return true;
			}
		};

		template<typename S, typename SQ = StringQueue>
		struct PrintStatic
		{
			MXT_CHECK_STRING(S);

			static void run(void)
			{
				SQ::push(S::toString());
			}
		};

		template<typename S, typename SQ = StringQueue>
		using Print = Hook<PrintStatic<S>>;

		template<typename SQ = StringQueue, typename SS = StringStore>
		struct PrintMatch
		{
			static void run(void)
			{
				SQ::push(SS::get());
			}
		};

		typedef Hook<PrintMatch<>> PrintID;

		template<typename S = ID_ALL_REGEX, typename SS = StringStore, bool Literal = false>
		struct Match
		{
			MXT_CHECK_STRING(S);

			template<typename I>
			static bool matches(I& input)
			{
				if(input.empty())
				{
					return false;
				}
				else if(Literal ? input.top() == S::toString()
								: MXT_REGEX::regex_match(input.top(), MXT_REGEX::regex(S::toString())))
				{
					SS::set(input.top());

					input.pop();

					return true;
				}

				return false;
			}
		};

		template<typename S, typename SS = StringStore>
		using Literal = Match<S, SS, true>;

#undef MXT_CHECK_STRING

// # ---------------------------------------------------------------------------
		
		struct FAIL { };

		template<int I>
		using Production = Number<I>;

		template<int I, typename T, typename TT>
		struct Trans;

		template<typename R, typename N>
		struct GetTrans
		{
			typedef Trans<N::value, typename Get<R, N::value>::type, R> type;
		};

		template<int Idx, typename T, typename TT, typename Reference>
		struct Trans<Idx, TypeList<T, TT>, Reference>
		{
			typedef typename T::head H;
			typedef typename If<IsDerived<Num, H>, GetTrans<Reference, H>, Identity<H>>::type head;

			typedef Trans<Idx, TypeList<typename T::tail, FAIL>, Reference> success;
			typedef Trans<Idx, TT, Reference> failure;

			template<typename I>
			static bool matches(I& input)
			{
				return head::matches(input) ? success::matches(input) : failure::matches(input);
			}
		};

		template<int N, typename R>
		struct Trans<N, TypeList<NIL, FAIL>, R>
		{
			template<typename I>
			static bool matches(I& input)
			{
				return true;
			}
		};

		template<int N, typename R>
		struct Trans<N, FAIL, R>
		{
			template<typename I>
			static bool matches(I& input)
			{
				throw aux::stringify("unexpected token #", input.pos(), " '", 
					input.empty() ? "#EOF#" : input.top(), "' in translation #", N);
			}
		};

		template<int N, typename R>
		struct Trans<N, NIL, R> : Number<N>
		{
			template<typename I>
			static bool matches(I& input)
			{
				return false;
			}
		};

// # ---------------------------------------------------------------------------

		template<typename RawList>
		struct Analysis
		{
			template<typename T, int I = 0>
			struct Make;

			template<typename H, typename T, int I>
			struct Make<TypeList<H, T>, I>
			{
				typedef TypeList<Trans<I, H, RawList>, typename Make<T, I + 1>::type> type;
			};

			template<int I>
			struct Make<NIL, I>
			{
				typedef NIL type;
			};

			typedef typename Make<RawList>::type Reference;
			typedef typename Reference::head Start;

			template<typename I>
			static void parse(I& input)
			{
				if(!Start::matches(input))
				{
					throw aux::stringify("failed to match. stopped at #", 
						input.pos(), ", '", input.empty() ? "#EOF#" : input.top(), "'");
				}
			}
		};
	}
}

#endif

