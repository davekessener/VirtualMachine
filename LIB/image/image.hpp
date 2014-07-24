#ifndef LIB_IMAGE_H
#define LIB_IMAGE_H

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
			typedef proxies::proxy<image<Color>, Color> proxy;
			typedef proxies::const_proxy<image<Color>, Color> const_proxy;
			public:
			typedef Color value_type;
			typedef Color *iterator;
			typedef const Color *const_iterator;

			public:
				image( );
				image(size_t, size_t);
				template<typename I>
					image(I, size_t, size_t);
				image(const image<Color>&);
				image(image<Color>&&);
				template<typename C>
					image(const image<C>&);
				virtual ~image( );
				image<Color>& operator=(const image<Color>&);
				image<Color>& operator=(image<Color>&&);
				template<typename C>
					image<Color>& operator=(const image<C>&);
				proxy operator[](size_t);
				const_proxy operator[](size_t) const;
				void set(size_t, size_t, const Color&);
				const Color& get(size_t, size_t) const;
				Color& at(size_t, size_t);
				Color *raw( );
				const Color *raw( ) const;
				bool valid( ) const;
				void swap(image<Color>&) noexcept;
				inline size_t width( ) const { return w_; }
				inline size_t height( ) const { return h_; }
				inline iterator begin( ) { return raw(); }
				inline iterator end( ) { return raw() + w_ * h_; }
				inline const_iterator begin( ) const { return raw(); }
				inline const_iterator end( ) const { return raw() + w_ * h_; }
				inline const_iterator cbegin( ) const { return raw(); }
				inline const_iterator cend( ) const { return raw() + w_ * h_; }
			protected:
				virtual void i_set(size_t, size_t, const Color&);
				virtual const Color& i_get(size_t, size_t) const;
				virtual Color& i_at(size_t, size_t);
				virtual Color *i_raw( );
				virtual const Color *i_raw( ) const;
				virtual bool i_valid( ) const;
				virtual void i_swap(image<Color>&) noexcept;
			private:
				Color *data_;
				size_t w_, h_;
		};

		template<typename C>
		image<C>::image(void) : data_(nullptr), w_(0), h_(0)
		{
		}

		template<typename C>
		image<C>::image(size_t w, size_t h) : data_(w * h ? new C[w * h] : nullptr), w_(w), h_(h)
		{
		}

		template<typename C>
		template<typename I>
		image<C>::image(I i, size_t w, size_t h) : image<C>(w, h)
		{
			std::copy(i, w_ * h_, data_);
		}

		template<typename C>
		image<C>::image(const image<C>& i) : image<C>(i.data_, i.w_, i.h_)
		{
		}

		template<typename C>
		image<C>::image(image<C>&& i) : data_(nullptr), w_(0), h_(0)
		{
			swap(i);
		}

		template<typename C1>
		template<typename C2>
		image<C1>::image(const image<C2>& i) : image<C1>(i.data_, i.w_, i.h_)
		{
		}

		template<typename C>
		image<C>::~image(void)
		{
			delete[] data_;
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
			assert(valid());
			return proxy(*this, i);
		}

		template<typename C>
		typename image<C>::const_proxy image<C>::operator[](size_t i) const
		{
			assert(valid());
			return const_proxy(data_ + i * w_);
		}

		template<typename C>
		void image<C>::set(size_t x, size_t y, const C& c)
		{
			assert(x<w_&&y<h_);
			assert(valid());
			i_set(x, y, c);
		}

		template<typename C>
		const C& image<C>::get(size_t x, size_t y) const
		{
			assert(x<w_&&y<h_);
			assert(valid());
			return i_get(x, y);
		}

		template<typename C>
		C& image<C>::at(size_t x, size_t y)
		{
			assert(x<w_&&y<h_);
			assert(valid());
			return i_at(x, y);
		}

		template<typename C>
		C *image<C>::raw(void)
		{
			assert(valid());
			return i_raw();;
		}

		template<typename C>
		const C *image<C>::raw(void) const
		{
			assert(valid());
			return i_raw();
		}

		template<typename C>
		bool image<C>::valid(void) const
		{
			return i_valid();
		}

		template<typename C>
		void image<C>::swap(image<C>& i) noexcept
		{
			i_swap(i);
		}

		template<typename C>
		void image<C>::i_set(size_t x, size_t y, const C& c)
		{
			data_[y * w_ + x] = c;
		}

		template<typename C>
		const C& image<C>::i_get(size_t x, size_t y) const
		{
			return data_[y * w_ + x];
		}

		template<typename C>
		C& image<C>::i_at(size_t x, size_t y)
		{
			return data_[y * w_ + x];
		}

		template<typename C>
		C *image<C>::i_raw(void)
		{
			return data_;
		}

		template<typename C>
		const C *image<C>::i_raw(void) const
		{
			return data_;
		}

		template<typename C>
		bool image<C>::i_valid(void) const
		{
			return data_ != nullptr;
		}

		template<typename C>
		void image<C>::i_swap(image<C>& i) noexcept
		{
			C *c = data_;
			data_ = i.data_;
			i.data_ = c;

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

