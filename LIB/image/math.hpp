#ifndef LIB_MATH_H
#define LIB_MATH_H

#include <array>
#include <iosfwd>
#include <iterator>
#include <algorithm>
#include <cassert>
#include "aux.hpp"

namespace lib
{
	namespace aux
	{
		template<typename T, size_t N, size_t M>
		struct matrix
		{
			static_assert(N > 0 && M > 0, "No matrix is able to have zero dimensions!");

			public:
				explicit matrix(T v = 0) { std::fill(c_.begin(), c_.end(), v); }
				template<size_t O>
					matrix<T, N, O> operator*(const matrix<T, M, O>&) const;
				matrix& operator*=(const T&);
				matrix& operator+=(const matrix<T, N, M>&);
				bound_arr<T, M> operator[](size_t i) { assert(i<N); return &c_[i * M]; }
				bound_arr<const T, M> operator[](size_t i) const { assert(i<N); return &c_[i * M]; }
				void print(std::ostream&) const;
			private:
				template<typename T2, size_t N2, size_t M2>
					friend class matrix;
				std::array<T, N * M> c_;
		};

		template<typename T, size_t N, size_t M>
		inline std::ostream& operator<<(std::ostream& os, const matrix<T, N, M>& m)
		{
			m.print(os);
			return os;
		}

		template<typename T>
		T abs(const T *t, size_t n)
		{
			T r(0);
			while(n--)
			{
				r += *t * *t;
				++t;
			}
			return static_cast<T>(std::sqrt(static_cast<double>(r)));
		}

		template<typename T, size_t N, size_t M>
		T abs(const matrix<T, N, M>& m)
		{
			static_assert(N == 1 || M == 1, "You can only take the absolute of an one-dimensional vector!");
			return abs(m[0], N * M);
		}

		template<typename T, size_t N, size_t M>
		matrix<T, N, M> operator*(const T& t, const matrix<T, N, M>& m)
		{
			matrix<T, N, M> o(m);
			o *= t;
			return o;
		}

		template<typename T, size_t N, size_t M>
		matrix<T, N, M> operator+(const matrix<T, N, M>& m1, const matrix<T, N, M>& m2)
		{
			matrix<T, N, M> m(m1);
			m += m2;
			return m;
		}

		template<typename T, size_t N, size_t M>
		template<size_t O>
		matrix<T, N, O> matrix<T, N, M>::operator*(const matrix<T, M, O>& m) const
		{
			matrix<T, N, O> o;

			for(size_t i = 0 ; i < N ; ++i)
			{
				for(size_t j = 0 ; j < O ; ++j)
				{
					T v(0);

					for(size_t k = 0 ; k < M ; ++k)
					{
						v += c_[k + i * M] * m.c_[j + k * O];
					}

					o.c_[j + i * O] = v;
				}
			}

			return o;
		}

		template<typename T, size_t N, size_t M>
		matrix<T, N, M>& matrix<T, N, M>::operator*=(const T& t)
		{
			std::transform(c_.begin(), c_.end(), [&t](const T& v) { return v * t; });
			return *this;
		}

		template<typename T, size_t N, size_t M>
		matrix<T, N, M>& matrix<T, N, M>::operator+=(const matrix<T, N, M>& m)
		{
			auto i = m.c_.cbegin();
			std::transform(c_.begin(), c_.end(), [&i](const T& v) { return v + *i++; });
			return *this;
		}

		template<typename T, size_t N, size_t M>
		void matrix<T, N, M>::print(std::ostream& os) const
		{
			os << "[" << N << " x " << M << "]:{";
			for(size_t n = 0 ; n < N ; ++n)
			{
				aux::ostream_iterator<T> i(os, ", ");
				os << "{";
				std::copy_n(&c_[n * M], M, i);
				os << "}";
			}
			os << "};";
		}
	}
}

#endif

