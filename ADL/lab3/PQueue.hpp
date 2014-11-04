#ifndef DAV_PQUEUE_H
#define DAV_PQUEUE_H

#include <algorithm>
#include <vector>
#include <cassert>

namespace dav
{
	template
	<
		typename T,
		typename S = std::less<T>,
		typename V = std::vector<T>
	>
	class PQueue
	{
		public:
		typedef V impl;
		typedef typename impl::value_type value_type;
		typedef typename impl::pointer pointer;
		typedef typename impl::const_pointer const_pointer;
		typedef typename impl::reference reference;
		typedef typename impl::const_reference const_reference;
		typedef typename impl::size_type size_type;
		typedef typename impl::difference_type difference_type;

		public:
			inline PQueue( ) { }
			explicit PQueue(const impl& v) : impl_(v) { order(); }
			explicit PQueue(impl&& v) : impl_(v) { order(); }
			template<typename I> PQueue(I i1, I i2) : impl_(i1, i2) { order(); }
			void insert(const_reference);
			void remove( );
			reference top( );
			const_reference top( ) const;
			size_type size( ) const;
			bool empty( ) const;
		private:
			void order( );
			void sink(size_type);
			void swim(size_type);
		private:
			impl impl_;
	};

	template<typename T, typename S = std::less<typename T::value_type>>
	using PriorityQueueFromContainer = PQueue<typename T::value_type, S, T>;

// # ===========================================================================

	template<typename T, typename S, typename V>
	void PQueue<T, S, V>::insert(const_reference v)
	{
		impl_.push_back(v);
		swim(size());
	}

	template<typename T, typename S, typename V>
	void PQueue<T, S, V>::remove(void)
	{
		if(size() > 1) std::swap(impl_.front(), impl_.back());
		impl_.pop_back();
		if(size() > 1) sink(1);
	}

	template<typename T, typename S, typename V>
	typename PQueue<T, S, V>::reference PQueue<T, S, V>::top(void)
	{
		return impl_.front();
	}

	template<typename T, typename S, typename V>
	typename PQueue<T, S, V>::const_reference PQueue<T, S, V>::top(void) const
	{
		return impl_.front();
	}

	template<typename T, typename S, typename V>
	typename PQueue<T, S, V>::size_type PQueue<T, S, V>::size(void) const
	{
		return impl_.size();
	}

	template<typename T, typename S, typename V>
	bool PQueue<T, S, V>::empty(void) const
	{
		return impl_.empty();
	}

// # ---------------------------------------------------------------------------

	template<typename T, typename S, typename V>
	void PQueue<T, S, V>::order(void)
	{
		for(size_type k(size() / 2) ; k > 0 ; --k) sink(k);
	}

	template<typename T, typename S, typename V>
	void PQueue<T, S, V>::sink(size_type k)
	{
		assert(k>0&&k<=size());

		size_type N(size());
		auto i(impl_.begin()); std::advance(i, k - 1);
		S op;

		while(2 * k <= N)
		{
			auto j(i); std::advance(j, k);
			
			k *= 2;

			if(k < N)
			{
				auto jj(j); ++jj;

				if(op(*jj, *j))
				{
					j = jj;
					++k;
				}
			}

			if(!op(*j, *i)) break;

			std::swap(*i, *j);
			
			i = j;
		}
	}

	template<typename T, typename S, typename V>
	void PQueue<T, S, V>::swim(size_type k)
	{
		assert(k>0&&k<=size());

		size_type N(size());
		auto i(impl_.rbegin()); std::advance(i, N - k);
		S op;

		while(k > 1)
		{
			size_type t((k + 1) / 2);

			if(!t) break;
			
			auto j(i); std::advance(j, t);
			
			if(!op(*i, *j)) break;
			
			std::swap(*i, *j);
			
			i = j;
			k /= 2;
		}
	}
}

#endif

