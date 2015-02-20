#ifndef DAV_LIB_STREAMQUEUE_H
#define DAV_LIB_STREAMQUEUE_H

#include "inc.h"

namespace dav
{
	namespace lib
	{
		template<typename T>
		struct Identity
		{
			T& operator()(T& t) const { return t; }
			const T& operator()(const T& t) const { return t; }
		};

		template
		<
			typename T,
			typename I = std::istream,
			typename O = std::ostream,
			typename S = Identity<T>
		>
		class StreamQueue
		{
			typedef T packet_t;
			typedef I input_t;
			typedef O output_t;
			typedef S access_t;
			typedef StreamQueue<T, I, O> self_t;
			typedef std::lock_guard<std::mutex> guard_t;

			public:
				StreamQueue(input_t& in) : StreamQueue(&in, nullptr) { }
				StreamQueue(output_t& out) : StreamQueue(nullptr, &out) { }
				StreamQueue(input_t& in, output_t& out) : StreamQueue(&in, &out) { }
				void push(const packet_t& p)
					{ guard_t lock(*mtx_); if(!out_) throw std::string("ERR: No output for packet!"); *out_ << access_t()(p); }
				packet_t poll( ) { guard_t lock(*mtx_); packet_t p(buf_->front()) buf_->poll(); return p; }
				bool empty( ) const { guard_t lock(*mtx_); return buf_->empty(); }
			private:
				StreamQueue(input_t *, output_t *);
			private:
				input_t *in_;
				output_t *out_;
				std::shared_ptr<std::mutex> mtx_;
				std::shared_ptr<std::queue<packet_t>> buf_;
		};

		template<typename T, typename I, typename O, typename S>
		StreamQueue::StreamQueue(input_t *in. output_t *out)
			: in_(in), out_(out)
		{
			if(in_)
			{
				
			}
		}
	}
}

#endif

