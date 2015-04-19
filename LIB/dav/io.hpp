#ifndef DAV_IO_H
#define DAV_IO_H

#include <string>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <cassert>

namespace dav
{
	template<typename I>
	struct deref_traits
	{
		I i;
		typedef decltype(*i) type;
	};

// # ===========================================================================

	namespace io
	{
		template<typename T>
		class ReadContainer
		{
			public:
				virtual ~ReadContainer( ) { }
				virtual size_t pos( ) const { return -1; }
				virtual T& top( ) = 0;
				virtual void pop( ) = 0;
				virtual bool empty( ) const = 0;
		};

		template<typename T, typename Alloc = std::allocator<T>>
		class IStreamContainer : public ReadContainer<T>
		{
			public:
				explicit IStreamContainer(std::istream_iterator<T> i) : i_(i), e_(std::istream_iterator<T>()), isb_(false), buf_(alloc_.allocate(1)) { }
				explicit IStreamContainer(std::istream& is) : IStreamContainer(std::istream_iterator<T>(is)) { }
				virtual ~IStreamContainer( ) { clear(); alloc_.deallocate(buf_, 1); }
				T& top( ) { check(); if(!isb_) { new(buf_) T(*i_); isb_ = true; } return *buf_; }
				void pop( ) { check(); clear(); ++i_; }
				bool empty( ) const { return i_ == e_; }
			private:
				void check( ) const { if(i_ == e_) throw std::string("istream empty!"); }
				void clear( ) { if(isb_) { isb_ = false; buf_->~T(); } }
			private:
				std::istream_iterator<T> i_, e_;
				Alloc alloc_;
				bool isb_;
				T *buf_;
		};

		template<typename I>
		class IteratorInput : public ReadContainer<typename deref_traits<I>::type>
		{
			public:
			typedef typename deref_traits<I>::type value_type;

			public:
				IteratorInput(const I& i1, const I& i2) : i1_(i1), i2_(i2), p_(0) { }
				virtual bool empty( ) const { return i1_ == i2_; }
				virtual size_t pos( ) const { return p_; }
				virtual const value_type& top( ) const { assert(i1_!=i2_); return *i1_; }
				virtual void pop( ) { assert(i1_!=i2_); ++i1_; ++p_; }
			private:
				I i1_, i2_;
				size_t p_;
		};

// # ===========================================================================

		template<typename T>
		class WriteContainer
		{
			public:
				virtual ~WriteContainer( ) { }
				virtual void reserve(size_t) { }
				virtual void push(const T&) = 0;
				virtual size_t size( ) const = 0;
			private:
		};

		template<typename T, void (T::*push_f)(const typename T::value_type&) = &T::push_back>
		class WriteSTLContainer : public WriteContainer<typename T::value_type>
		{
			public:
			typedef typename T::value_type value_type;

			public:
				WriteSTLContainer(T& t) : t_(t) { }
				void push(const value_type& t) { (t_.*push_f)(t); }
				size_t size( ) const { return t_.size(); }
			private:
				T &t_;
		};

		template<typename T>
		class OStreamContainer : public WriteContainer<T>
		{
			public:
				explicit OStreamContainer(std::ostream_iterator<T> i) : i_(i), s_(0) { }
				explicit OStreamContainer(std::ostream& os) : i_(std::ostream_iterator<T>(os)), s_(0) { }
				void push(const T& t) { *i_ = t; ++i_; ++s_; }
				size_t size( ) const { return s_; }
			private:
				std::ostream_iterator<T> i_;
				size_t s_;
		};
	}
}

#endif

