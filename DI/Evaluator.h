#ifndef DAV_DIL_EVALUATOR_H
#define DAV_DIL_EVALUATOR_H

#include <iostream>

namespace dav
{
	namespace dil
	{
		template<typename T>
		struct do_decay
		{
			typedef T type;
		};

		template<typename T> struct do_decay<T&> : do_decay<T> { };
		template<typename T> struct do_decay<const T> : do_decay<T> { };
		template<typename T> struct do_decay<volatile T> : do_decay<T> { };

		template<typename T>
		using decay = typename do_decay<T>::type;

		template<typename T1, typename T2>
		struct is_convertible
		{
			typedef decay<T1> T;
			typedef decay<T2> TT;

			struct A { char v[1]; };
			struct B { char v[2]; };
			static A fun(TT);
			template<typename C> static B fun(C);

			enum { value = sizeof(fun(*((T *) nullptr))) == sizeof(A) };
		};

		template<int V, typename T>
		struct do_enable_if_impl
		{
			typedef T type;
		};

		template<typename T>
		struct do_enable_if_impl<0, T>
		{
		};

		template<typename T, typename TT = void>
		struct do_enable_if : do_enable_if_impl<T::value, TT>
		{
		};

		template<typename T>
		struct do_deref_t
		{
			class impl
			{
				friend class do_deref_t<T>;

				static auto fun(T *t = nullptr) -> decltype(**t);
			};

			typedef decltype(impl::fun()) type;
		};

		template<typename T, typename TT = void>
		using enable_if = typename do_enable_if<T, TT>::type;

		template<typename T>
		using deref_t = typename do_deref_t<T>::type;

		class Evaluator
		{
			public:
				void parse(const std::string&);
				void out(std::ostream&) const;
			public:
				Evaluator( );
				Evaluator(const Evaluator&);
				Evaluator(Evaluator&&) noexcept;
				~Evaluator( ) noexcept;
				Evaluator& operator=(const Evaluator&);
				Evaluator& operator=(Evaluator&&) noexcept;
				void swap(Evaluator&) noexcept;
			private:
				struct Impl;
				Impl *impl_;
		};

		template<typename I>
		void evaluate(I i1, I i2, std::ostream& os = std::cout)
		{
			static_assert(is_convertible<deref_t<I>, std::string>::value, "*I has to be convertible to std::string!");

			Evaluator ev;

			while(i1 != i2)
			{
				ev.parse(*i1);
				++i1;
			}

			ev.out(os);
		}

		inline std::ostream& operator<<(std::ostream& os, const Evaluator& ev)
		{
			ev.out(os);
			return os;
		}
	}
}

#endif

