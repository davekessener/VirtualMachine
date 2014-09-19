#ifndef DAV_TYPES_H
#define DAV_TYPES_H

#include <boost/lexical_cast.hpp>
#include "lex.h"

using boost::lexical_cast;

namespace dav
{
	using lex::Hook;
	using lex::HookBase;
	using lex::HookImpl;
	using lex::PrintID;
	using lex::Literal;

	typedef double number_t;

	typedef String<'('> OPEN_PARA_S;
	typedef String<')'> CLOSE_PARA_S;
	typedef String<'+'> ADD_S;
	typedef String<'-'> SUB_S;
	typedef String<'*'> MUL_S;
	typedef String<'/'> DIV_S;
	typedef String<'[', '0', '-', '9', ']', '+', '(', '\\', '.', '[', '0', '-', '9', ']', '+', ')', '?', '(', '[', 'e', 'E', ']', '[', '+', '-', ']', '?', '[', '0', '-', '9', ']', '+', ')', '?'> ID_DEC_REGEX;
	
	typedef String<'[', '0', '-', '9', ']'> ID_DIGIT_REGEX;
	typedef String<'.'> DOT_S;
	typedef String<'[', 'e', 'E', ']'> ID_EXP_REGEX;
	
	typedef String<'[', '\\', 't', ' ', ']'> ID_WS_REGEX;
	typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '_', ']'> ID_IDT_BEG_REGEX;
	typedef String<'[', 'a', '-', 'z', 'A', '-', 'Z', '0', '-', '9', '_', ']'> ID_IDT_END_REGEX;
	typedef String<'[', '\\', '`', '\\', '~', '\\', '!', '\\', '@', '\\', '#', '\\', '$', '\\', '%', '\\', '^', '\\', '&', '\\', '*', '\\', '(', '\\', ')', '\\', '-', '\\', '_', '\\', '=', '\\', '+', '\\', '[', '\\', ']', '\\', '{', '\\', '}', '\\', '\'', '\\', '"', '\\', '\\', '\\', '|', '\\', '/', '\\', '?', '\\', '.', '\\', '>', '\\', ',', '\\', '<', ']'> ID_OP_REGEX;
	
	template<int V>
	using Translation = Number<V>;
	
	typedef PrintID Push;

	template<typename ST, typename O>
	struct Finalizer
	{
		static void run(ST& st, O& o)
		{
			o.finalize();
		}
	};
	typedef Hook<Finalizer> Finalize;
	
	template<typename S = std::string, typename Container = std::vector<S>>
	class StreamVec
	{
		public:
		typedef S value_type;
		typedef Container vec_t;
		typedef typename vec_t::iterator iterator;
		typedef typename vec_t::const_iterator const_iterator;
	
		public:
			virtual ~StreamVec( ) { }
			StreamVec& operator<<(const value_type& s) { if(!s.empty()) v_.push_back(s); return *this; }
			iterator begin( ) { return v_.begin(); }
			iterator end( ) { return v_.end(); }
			const_iterator begin( ) const { return v_.cbegin(); }
			const_iterator end( ) const { return v_.cend(); }
			const_iterator cbegin( ) const { return v_.cbegin(); }
			const_iterator cend( ) const { return v_.cend(); }
			size_t size( ) const { return v_.size(); }
			bool empty( ) const { return v_.empty(); }
		private:
			vec_t v_;
	};
	typedef StreamVec<> SVec;
	
	template<typename S = std::string, typename Container = std::vector<S>>
	class StreamBuffer : public StreamVec<S, Container>
	{
		typedef typename StreamVec<S, Container>::value_type value_type;
		typedef typename StreamVec<S, Container>::vec_t vec_t;

		public:
			StreamBuffer<S, Container>& operator<<(const value_type& s) { tmp_.push_back(s); return *this; }
			void finalize( );
		private:
			vec_t tmp_;
	};
	typedef StreamBuffer<> Printer;
	
	template<typename S, typename C>
	void StreamBuffer<S, C>::finalize(void)
	{
		std::stringstream ss;
		
		for(const std::string& s : tmp_)
		{
			ss << s;
		}
		
		static_cast<StreamVec<S, C>&>(*this) << ss.str();
		
		tmp_.clear();
	}

	template<typename T>
	class StackHolderImpl
	{
		public:
		typedef Stack<T> stack_t;
	
		public:
			StackHolderImpl<T>& operator<<(const std::string& s) { s_.push(lexical_cast<T>(s)); return *this; }
			stack_t& get( ) { return s_; }
		private:
			stack_t s_;
	};
	typedef StackHolderImpl<number_t> StackHolder;
	
	template<typename T>
	struct Execute : HookBase
	{
		template<typename ST, typename O>
		struct Do
		{
			static void run(ST& st, O& o)
			{
				T::run(o.get());
			}
		};
	
		template<typename ST, typename O>
		struct Make
		{
			typedef HookImpl<ST, O, Do> type;
		};
	};
	
	template<typename S, template<typename> class BOP, typename E = ErrorThrow<>>
	struct SecureOP
	{
		typedef typename S::value_type value_type;
		typedef BOP<value_type> op_type;
		typedef typename E::error_type error_type;
		enum { operand_count = op_type::operand_count };
	
		static void run(S& st)
		{
			if(st.size() < operand_count)
			{
				E::OnError("Operation '" + GetName<op_type>::toString()
					+ "' failed, stacksize < " + lexical_cast<std::string>(operand_count)
					+ " (" + lexical_cast<std::string>(st.size()) + ")");
			}
	
			value_type t1(st.top()); st.pop();
			value_type t2(st.top()); st.pop();
			value_type r(op_type::run(t2, t1));
			st.push(r);
		}
	};
	
	template<int C> struct OPN { enum { operand_count = C }; };
	template<typename T> struct Addition       : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 + t2; } };
	template<typename T> struct Subtraction    : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 - t2; } };
	template<typename T> struct Multiplication : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 * t2; } };
	template<typename T> struct Division       : OPN<2> { static constexpr T run(const T& t1, const T& t2) { return t1 / t2; } };
	
	typedef SecureOP<StackHolder::stack_t, Addition> Add;
	typedef SecureOP<StackHolder::stack_t, Subtraction> Sub;
	typedef SecureOP<StackHolder::stack_t, Multiplication> Mul;
	typedef SecureOP<StackHolder::stack_t, Division> Div;
}

#endif

