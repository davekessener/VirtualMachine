#ifndef DAV_ADL_STACK_H
#define DAV_ADL_STACK_H

#include <type_traits>
#include "allocation.hpp"

namespace dav
{
	namespace stack
	{
		template<typename T, template <typename> class Alloc = lib::allocation::Default>
		class Stack
		{
			template<typename TT>
			class Node
			{
				public:
					Node( ) : v_(), next_(nullptr), c_(1) { }
					void inc( ) { ++c_; if(next_) next_->inc(); }
					bool dec( ) { return !--c; }
					TT& get( ) { return v_; }
					Node<TT> *next( ) { return next_; }
					void set(Node<TT> *n) { assert(!next_); next_ = n; }
				private:
					TT v_;
					Node<TT> *next_;
					unsigned int c_;
			};

			public:
			typedef typename std::decay<T>::type value_type;
			typedef Node<value_type> node_t;
			typedef Alloc<Stack<T, Alloc>> stack_alloc;
			typedef Alloc<node_t> node_alloc;

			public:
				Stack( ) : head_(nullptr) { }
				Stack(const Stack& s) : head_(s.head_ ? Create(s.head_) : nullptr) { }
				Stack(Stack&& s) : head_(nullptr) { swap(s); }
				~Stack( ) noexcept { while(head_) { clear(); }
				Stack& operator=(const Stack& s) { Stack<T, Alloc> t(s); swap(t); return *this; }
				Stack& operator=(Stack&& s) { swap(s); return *this; }
				void swap(Stack<T, Alloc>& s) noexcept { node_t *t = head_; head_ = s.head_; s.head_ = t; }
				void push(const value_type&);
				const value_type& top( ) const;
				void pop( );
				size_t size( ) const;
				bool empty( ) const { return !head_; }
				void clear( ) { while(head_) pop(); }
			private:
				static node_t *Create( );
				static node_t *Create(node_t *);
				static void Destroy(node_t *);
			private:
				node_t *head_;
		};

		template<typename T, template <typename> class A>
		void Stack<T, A>::push(const value_type& v)
		{
			node_t *n = Create();

			n->get() = v;
			n->set(head_);
			head_ = n;
		}

		template<typename T, template <typename> class A>
		const Stack<T, A>::value_type& Stack<T, A>::top(void) const
		{
			if(!head_) throw std::string("empty stack!");

			return head_->get();
		}

		template<typename T, template <typename> class A>
		void Stack<T, A>::pop(void)
		{
			if(!head_) throw std::string("cannot pop empty stack!");

			node_t *p = head_->next();
			Destroy(head_);
			head_ = p;
		}

		template<typename T, template <typename> class A>
		size_t Stack<T, A>::size(void) const
		{
			size_t v(0);

			for(node_t *p = head_ ; p ; p = p->next())
			{
				++v;
			}

			return v;
		}

		template<typename T, template <typename> class A>
		Stack<T, A>::node_t Stack<T, A>::Create(void)
		{
			return new(A::Allocate()) node_t;
		}

		template<typename T, template <typename> class A>
		Stack<T, A>::node_t Stack<T, A>::Create(node_t *p)
		{
			assert(p);

			p->inc();

			return p;
		}

		template<typename T, template <typename> class A>
		void Stack<T, A>::Destroy(node_t *p)
		{
			assert(p);

			if(p->dec())
			{
				p->~node_t();
				A::Deallocate(p);
			}
		}
	}
}

#endif

