#ifndef LIB_IMAGE_COLORS_H
#define LIB_IMAGE_COLORS_H

namespace lib
{
	namespace img
	{
		typedef std::uint8_t BYTE;
		typedef std::uint16_t WORD;
		typedef std::uint32_t DWORD;
		typedef std::uint64_t QWORD;

		template<typename T, size_t L>
		struct parr
		{
			T v_[L];
		} __attribute__((packed));

		struct X
		{
			explicit X(QWORD) { }
			void operator*(double) { }
			void operator+(QWORD) { }
			explicit operator QWORD( ) const { return 0; }
		} __attribute__((packed));

#define TRUNC(v) ((((v)<<(sizeof(T)*8-U))&static_cast<T>(-1))>>(sizeof(T)*8-U))
#define COL(N,n) \
template<typename T, size_t U, typename S> \
struct N : public S \
{ \
	public: \
		explicit N(QWORD v) : S(v >> U), n##_(TRUNC(v)) { } \
		inline int n( ) const { return n##_; } \
		void operator*(double v) { n##_ = v < 0 ? 0 : TRUNC(static_cast<int>(v * n##_)); S::operator*(v); } \
		void operator+(QWORD v) { n##_ = TRUNC(TRUNC(v) + n##_); S::operator+(v >> U); } \
		explicit operator QWORD( ) const { return ((S::operator QWORD()) << U) | n##_; } \
	private: \
		T n##_; \
} __attribute__((packed));

		COL(R,r);
		COL(G,g);
		COL(B,b);
		COL(A,a);
#undef COL
#undef TRUNC

		template<typename T>
		struct C : public T
		{
			typedef C<T> value_type;
			
			explicit C(QWORD v = 0) : T(v) { }
			value_type value(void) const { return *this; }
			value_type operator*(double v) const { value_type t(*this); t.T::operator*(v); return t; }
			value_type operator+(const value_type& v) const { value_type t(*this); t.T::operator+(static_cast<QWORD>(v)); return t; }
		} __attribute__((packed));

		template<typename T> struct RGB  { typedef C<R<T, sizeof(T) * 8, G<T, sizeof(T) * 8, B<T, sizeof(T) * 8, X>>>> value_type; };
		template<typename T> struct RGBA { typedef C<R<T, sizeof(T) * 8, G<T, sizeof(T) * 8, B<T, sizeof(T) * 8, A<T, sizeof(T) * 8, X>>>>> value_type; };

		typedef typename RGB<BYTE>::value_type  RGB8;
		typedef typename RGB<WORD>::value_type  RGB16;
		typedef typename RGBA<BYTE>::value_type RGBA8;
		typedef typename RGBA<WORD>::value_type RGBA16;

		template<typename T> inline typename C<T>::value_type operator*(double v, const C<T>& c) { return c * v; }
	}
}

#endif

