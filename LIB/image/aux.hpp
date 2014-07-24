#ifndef LIB_IMAGE_AUX_H
#define LIB_IMAGE_AUX_H

#include <iosfwd>

namespace lib
{
	namespace aux
	{
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
	}
}

#endif

