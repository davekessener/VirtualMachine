#ifndef LIB_IMAGE_AUX_H
#define LIB_IMAGE_AUX_H

#include <iosfwd>
#include <sstream>

namespace lib
{
	namespace aux
	{
		template<typename T, size_t N>
		struct bound_arr
		{
			public:
				bound_arr(T *t) : t_(t) { }
				T& operator[](size_t i) { assert(i < N); return t_[i]; }
				T& operator*(void) { return operator[](0); }
			private:
				T *t_;
		};

		template<typename T>
		class ostream_iterator
		{
			struct helper
			{
				public:
					helper(std::ostream& os, const std::string& delim) : os_(os), delim_(delim) { }
					void operator=(const T& t) { os_ << delim_ << t; }
				private:
					std::ostream& os_;
					std::string delim_;
			};

			public:
				explicit ostream_iterator(std::ostream& os) : os_(os), delim_(""), first_(0) { }
				ostream_iterator(std::ostream& os, const std::string& s) : os_(os), delim_(s), first_(0) { }
				helper operator*(void) { return helper(os_, first_++ ? delim_ : ""); }
				ostream_iterator& operator++(void) { return *this; }
			private:
				std::ostream& os_;
				std::string delim_;
				size_t first_;
		};

		template<typename II, typename OI>
		void copy(OI&& oi, II i1, II i2)
		{
			while(i1 != i2)
			{
				*oi = *i1;
				++oi;
				++i1;
			}
		}

		template<typename II, typename OI>
		void copy_n(OI&& oi, II&& ii, size_t n)
		{
			while(n--)
			{
				*oi = *ii;
				++oi;
				++ii;
			}
		}

		template<typename T, typename S>
		T lexical_cast(const S& s)
		{
			T t;
			std::stringstream ss;

			if(!(ss << s) || !(ss >> t) || !(ss >> std::ws).eof()) throw std::string("Bad lexical cast.");

			return t;
		}

		template<typename II, typename OI, typename F>
		void transform_n(II&& i, size_t n, OI&& o, F f)
		{
			while(n--)
			{
				*o = f(*i);
				++i;
				++o;
			}
		}

		template<typename I>
		struct deref_trait
		{
			I i;
			typedef decltype(*i) type;
		};

// # ===========================================================================

		template<typename T>
		struct os_write_to_container
		{
			typedef T value_type;
			os_write_to_container(const T *t, size_t n) : t_(t), n_(n) { }
			const T *t_;
			size_t n_;
		};

		template<typename T>
		os_write_to_container<T> write_to(const T& t, size_t n = sizeof(T))
		{
			return os_write_to_container<T>(&t, n);
		}

		template<typename T>
		inline std::ostream& operator<<(std::ostream& os, const os_write_to_container<T>& c)
		{
			os.write(reinterpret_cast<const char *>(c.t_), c.n_);
			return os;
		}

// # ===========================================================================
		
		template<typename T>
		struct is_read_from_container
		{
			typedef T value_type;
			is_read_from_container(T *t, size_t n) : t_(t), n_(n) { }
			T *t_;
			size_t n_;
		};

		template<typename T>
		is_read_from_container<T> read_from(T& t, size_t n = sizeof(T))
		{
			return is_read_from_container<T>(&t, n);
		}

		template<typename T>
		inline std::istream& operator>>(std::istream& is, is_read_from_container<T>&& c)
		{
			is.read(reinterpret_cast<char *>(c.t_), c.n_);
			return is;
		}
	}
}

namespace std
{
	template<typename T>
	struct iterator_traits<lib::aux::ostream_iterator<T>>
	{
		typedef T value_type;
	};
}

#endif

