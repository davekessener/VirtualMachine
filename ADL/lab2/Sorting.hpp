#ifndef DAV_SORTING_H
#define DAV_SORTING_H

#include <algorithm>

#ifndef NDEBUG
#	include <memory>
#	define MXT_SORTING_SWAP(a,b) swap((a),(b))
#	define MXT_SORTING_MIN_ELEMENT(a,b) min_element((a),(b))
#	define MXT_SORTING_UPPER_BOUND(a,b,c) upper_bound((a),(b),(c))
#	define MXT_SORTING_COPY(a,b,c) copy((a),(b),(c))
#	define MXT_SORTING_INCOP ++*opc_
#	define MXT_SORTING_CLASS(name) \
		public: \
			name( ) : opc_(new size_t(0)) { } \
			size_t getOPCount( ) const { return *opc_; } \
			void reset( ) { *opc_ = 0; } \
		private: \
			template<typename S_I_> \
			S_I_ min_element(S_I_ i1, S_I_ i2) \
			{ \
				S_I_ i(i1); \
				S op; \
				while(i1 != i2) \
				{ \
					if(op(*i1, *i)) i = i1; \
					++i1; \
					++*opc_; \
				} \
				return i; \
			} \
			template<typename S_T_> \
			void swap(S_T_& v1, S_T_& v2) \
			{ \
				std::swap(v1, v2); \
				++*opc_; \
			} \
			template<typename S_I_, typename S_T_> \
			S_I_ upper_bound(S_I_ i1, S_I_ i2, const S_T_& v) \
			{ \
				S_I_ i(i1); \
				typename std::iterator_traits<S_I_>::difference_type span(std::distance(i1, i2)); \
				S op; \
				while(span) \
				{ \
					std::advance(i, span >> 1); \
					++*opc_; \
					if(op(v,*i)) \
					{ \
						i1 = ++i; \
						span = (span - 1) >> 1; \
					} \
					else \
					{ \
						i = i1; \
						span >>= 1; \
					} \
				} \
				return i1; \
			} \
			template<typename S_II_, typename S_OI_> \
			void copy(S_II_ i1, S_II_ i2, S_OI_ oi) \
			{ \
				while(i1 != i2) \
				{ \
					*oi = *i1; \
					++i1; \
					++oi; \
					++*opc_; \
				} \
			} \
		private: \
			std::shared_ptr<size_t> opc_
#else
#	define MXT_SORTING_SWAP(a,b) std::swap((a),(b))
#	define MXT_SORTING_MIN_ELEMENT(a,b) std::min_element((a),(b),S())
#	define MXT_SORTING_UPPER_BOUND(a,b,c) std::upper_bound((a),(b),(c),S())
#	define MXT_SORTING_COPY(a,b,c) std::copy((a),(b),(c))
#	define MXT_SORTING_INCOP
#	define MXT_SORTING_CLASS(name)
#endif

#endif

