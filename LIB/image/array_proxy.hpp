#ifndef LIB_ARRAYPROXY_H
#define LIB_ARRAYPROXY_H

namespace lib
{
	namespace proxies
	{
		template<typename T, typename TT>
		struct assign_proxy
		{
			public:
				assign_proxy(T& t, size_t i, size_t j) : t_(t), i_(i), j_(j) { }
				assign_proxy(assign_proxy& p) : t_(p.t_), i_(p.i_), j_(p.j_) { }
				assign_proxy(assign_proxy&& p) : t_(p.t_), i_(p.i_), j_(p.j_) { }
				const TT operator=(const TT& c) { t_.set(i_, j_, c); return t_.get(i_, j_); }
				operator TT&(void) { return t_.at(i_, j_); }
			private:
				T &t_;
				size_t i_, j_;
		};

		template<typename T, typename TT>
		struct proxy
		{
			public:
				proxy(T& t, size_t i) : t_(t), i_(i) { }
				proxy(proxy& p) : t_(p.t_), i_(p.i_) { }
				proxy(proxy&& p) : t_(p.t_), i_(p.i_) { }
				assign_proxy<T, TT> operator[](size_t j) { return assign_proxy<T, TT>(t_, i_, j); }
				assign_proxy<T, TT> operator*(void) { return (*this)[0]; }
			private:
				T &t_;
				size_t i_;
		};

		template<typename T, typename TT>
		struct const_proxy
		{
			public:
				const_proxy(const TT *c) : c_(c) { }
				const TT& operator[](size_t i) { return c_[i]; }
				operator const TT *(void) { return c_; }
				const TT& operator*(void) { return *c_; }
			private:
				const TT *c_;
		};
	}
}

#endif

