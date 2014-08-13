#ifndef LIB_IMAGE_H
#define LIB_IMAGE_H

#include <vector>
#include <algorithm>
#include <cassert>
#include "array_proxy.hpp"

namespace lib
{
	namespace img
	{
		template<typename Color>
		class image
		{
			typedef std::vector<Color> ccon_t;
			typedef proxies::proxy<image<Color>, Color> proxy;
			typedef proxies::const_proxy<image<Color>, Color> const_proxy;
			public:
			typedef Color color_type;
			typedef typename Color::value_type data_type;
			typedef typename ccon_t::iterator iterator;
			typedef typename ccon_t::const_iterator const_iterator;

			public:
				image( );
				image(size_t, size_t);
				template<typename I>
					image(I, size_t, size_t);
				image(const image<Color>&);
				image(image<Color>&&);
				template<typename C>
					image(const image<C>&);
				image<Color>& operator=(const image<Color>&);
				image<Color>& operator=(image<Color>&&);
				template<typename C>
					image<Color>& operator=(const image<C>&);
				proxy operator[](size_t);
				const_proxy operator[](size_t) const;
				void set(size_t, size_t, const Color&);
				const Color& get(size_t, size_t) const;
				void fill(const Color&);
				Color& at(size_t, size_t);
				Color *raw( );
				const Color *raw( ) const;
				bool valid( ) const;
				void swap(image<Color>&) noexcept;
				inline size_t width( ) const { return w_; }
				inline size_t height( ) const { return h_; }
				inline iterator begin( ) { assert(valid()); return data_.begin(); }
				inline iterator end( ) { assert(valid()); return data_.end(); }
				inline const_iterator begin( ) const { assert(valid()); return data_.cbegin(); }
				inline const_iterator end( ) const { assert(valid()); return data_.cend(); }
				inline const_iterator cbegin( ) const { assert(valid()); return data_.cbegin(); }
				inline const_iterator cend( ) const { assert(valid()); return data_.cend(); }
			private:
				ccon_t data_;
				size_t w_, h_;
		};

		template<typename C>
		image<C>::image(void) : data_(), w_(0), h_(0)
		{
		}

		template<typename C>
		image<C>::image(size_t w, size_t h) : data_(w * h), w_(w), h_(h)
		{
		}

		template<typename C>
		template<typename I>
		image<C>::image(I i, size_t w, size_t h) : image<C>(w, h)
		{
			std::copy_n(i, w_ * h_, data_);
		}

		template<typename C>
		image<C>::image(const image<C>& i) : image<C>(i.data_, i.w_, i.h_)
		{
		}

		template<typename C>
		image<C>::image(image<C>&& i) : data_(), w_(0), h_(0)
		{
			swap(i);
		}

		template<typename C1>
		template<typename C2>
		image<C1>::image(const image<C2>& i) : image<C1>(i.data_, i.w_, i.h_)
		{
		}

		template<typename C>
		image<C>& image<C>::operator=(const image<C>& i)
		{
			image<C> t(i);
			swap(t);
			return *this;
		}

		template<typename C>
		image<C>& image<C>::operator=(image<C>&& i)
		{
			swap(i);
			return *this;
		}

		template<typename C1>
		template<typename C2>
		image<C1>& image<C1>::operator=(const image<C2>& i)
		{
			image<C1> t(i);
			swap(t);
			return *this;
		}

		template<typename C>
		typename image<C>::proxy image<C>::operator[](size_t i)
		{
			assert(i<w_);
			assert(valid());
			return proxy(*this, i);
		}

		template<typename C>
		typename image<C>::const_proxy image<C>::operator[](size_t i) const
		{
			assert(i<w_);
			assert(valid());
			return const_proxy(*this, i);
		}

		template<typename C>
		void image<C>::set(size_t x, size_t y, const C& c)
		{
			assert(valid());
			at(x, y) = c;
		}

		template<typename C>
		const C& image<C>::get(size_t x, size_t y) const
		{
			assert(valid());
			return data_.at(y * w_ + x);
		}

		template<typename C>
		void image<C>::fill(const C& c)
		{
			assert(valid());
			std::fill(begin(), end(), c);
		}

		template<typename C>
		C& image<C>::at(size_t x, size_t y)
		{
			assert(valid());
			return data_.at(y * w_ + x);
		}

		template<typename C>
		C *image<C>::raw(void)
		{
			assert(valid());
			return &*data_.begin();
		}

		template<typename C>
		const C *image<C>::raw(void) const
		{
			assert(valid());
			return &*data_.cbegin();
		}

		template<typename C>
		bool image<C>::valid(void) const
		{
			return data_.size() == w_ * h_;
		}

		template<typename C>
		void image<C>::swap(image<C>& i) noexcept
		{
			std::swap(data_, i.data_);

			size_t t = w_;
			w_ = i.w_;
			i.w_ = t;

			t = h_;
			h_ = i.h_;
			i.h_ = t;
		}
	}
}

#endif

