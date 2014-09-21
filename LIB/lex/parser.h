#ifndef DAV_PARSER_H
#define DAV_PARSER_H

#include <iosfwd>
#include <iterator>

namespace dav
{
	template<typename T>
	class ReadContainer
	{
		public:
		static const auto npos(std::string::npos);

		public:
			virtual ~ReadContainer( ) { }
			virtual size_t size( ) const { return npos; }
			virtual T& top( ) = 0;
			virtual void pop( ) = 0;
			virtual bool empty( ) const = 0;
	};

	template<typename T, typename Alloc = std::Allocator<T>>
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

#endif

