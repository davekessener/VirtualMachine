#ifndef DAV_BST_H
#define DAV_BST_H

#include <type_traits>
#include "STA.h"

#define MXT_MAX_STA_SIZE 64

namespace dav
{
	namespace bst
	{
		template<typename T, size_t V>
		struct WImpl
		{
			typedef T type;
		};

		template<typename T>
		struct WImpl<T, 0>
		{
			struct type
			{
				type(const T& t) : d_(new T(t)) { }
				~type( ) { delete d_; }
				operator T&( ) { return *d_; }

				T *d_;
			};
		};

		template<size_t V1, size_t V2>
		struct Smaller
		{
			enum { value = V1 > V2 };
		};

		template<typename T>
		struct Wrapper
		{
			typedef typename WImpl<T, Smaller<sizeof(T), MXT_MAX_STA_SIZE>::value>::type type;
		};

		template<typename P, typename K, typename V, typename S, typename C>
		P insert(P n, K k, const V& v, S&& op, C&& create)
		{
			P p(nullptr), r(n);
			P *np(&n);

			while(static_cast<bool>(*np))
			{
				bool f;

				p = *np;

				if(!(f = op((*np)->key(), k)) && !op(k, (*np)->key()))
				{
					(*np)->value() = v; break;
				}
				else
				{
					np = f ? &(*np)->left() : &(*np)->right();
				}
			}

			if(!static_cast<bool>(*np)) *np = create(k, v, p);

			return static_cast<bool>(r) ? r : *np;
		}
	
		template<typename P, typename K, typename S>
		P find(P n, K k, S&& op)
		{
			bool f;

			while(static_cast<bool>(n) && ((f = op(n->key(), k)) || op(k, n->key())))
			{
				n = f ? n->left() : n->right();
			}

			return n;
		}

		template<typename P, typename K, typename V, typename S, typename C>
		P insert_recursive(P n, K k, const V& v, S&& op, C&& create, P p = nullptr)
		{
			typedef typename std::decay<decltype(*n)>::type node_t;
			bool f;
			P& (node_t::*fn)();

			return   static_cast<bool>(n)
				   ? (   (
				             (!(f = op(n->key(), k)) && !op(k, n->key()))
				           ? (n->value() = v, n)
				           : (   fn = f ? &node_t::left : &node_t::right
				               , (n->*fn)() = insert_recursive((n->*fn)(), k, v, op, create, n))
				         )
				       , n)
				   : create(k, v, p);
		}

		template<typename P, typename K, typename S>
		P find_recursive(P n, K k, S&& op)
		{
			bool f;

			return   static_cast<bool>(n) && ((f = op(n->key(), k)) || op(k, n->key()))
				   ? find_recursive(f ? n->left() : n->right(), k, op)
				   : n;
		}

		template<typename T>
		struct Ident
		{
			typedef T result_type;

			const T& operator()(const T& t) const { return t; }
		};

		template<typename P>
		P next(P n, std::function<P(P)> left, std::function<P(P)> right, std::function<P(P)> parent)
		{
			if(static_cast<bool>(right(n)))
			{
				n = right(n);
				while(static_cast<bool>(left(n))) n = left(n);
			}
			else
			{
				P p(parent(n));

				while(static_cast<bool>(p) && left(p) != n)
				{
					n = p;
					p = parent(n);
				}

				n = p;
			}

			return n;
		}

		template<typename K, typename V>
		class Node : public SmallType
		{
			typedef typename Wrapper<K>::type key_storage_type;
			typedef typename Wrapper<V>::type value_storage_type;
			typedef Node<K, V> self;
			public:
			typedef K key_type;
			typedef V value_type;
			typedef value_type *pointer;
			typedef const value_type *const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
//			typedef typename std::shared_ptr<self> node_ptr;
			typedef self *node_ptr;

			public:
				Node(const key_type& k, const_reference v, node_ptr p) : key_(k), value_(v), parent_(p) { }
				~Node( ) { delete left_; delete right_; }
				const key_type& key( ) { return key_; }
				reference value( ) { return value_; }
				node_ptr& left( ) { return left_; }
				node_ptr& right( ) { return right_; }
			private:
				key_storage_type key_;
				value_storage_type value_;
				node_ptr left_ = nullptr, right_ = nullptr, parent_;
		};

		template<typename P>
		class BSTIter
		{
			typedef typename std::decay<decltype(*root_)>::type node_t;
			typedef BSTIter<P> self;
			public:
			typedef typename node_t::value_type value_type;
			typedef typename node_t::pointer pointer;
			typedef typename node_t::reference reference;
			typedef typename node_t::const_pointer const_pointer;
			typedef typename node_t::const_reference const_reference;
			typedef typename node_t::size_type size_type;
			typedef typename node_t::difference_type difference_type;
			
			public:
				BSTIter(P r) : root_(p), p_(p)
					{ assert(static_cast<bool>(root_); while(static_cast<bool>(p_->left())) p_ = p_->left(); }
				reference operator*( ) { assert(static_cast<bool>(p_)); return p_->value(); }
				pointer operator->( ) { assert(static_cast<bool>(p_)); return &p_->value(); }
				self& operator++( ) { p_ = next(p_, [](P p) { return p->left(); }, [](P p) { return p->right(); }, [](P p) { return p->parent(); }); }
				self& operator--( ) { p_ = next(p_, [](P p) { return p->right(); }, [](P p) { return p->left(); }, [](P p) { return p->parent(); }); }
				self operator++(int) { self t(*this); ++*this; return t; }
				self operator--(int) { self t(*this); --*this; return t; }
			private:
				P root_, p_;
		};
	}

	template<typename K, typename V, typename R = bst::Ident<K>, typename S = std::less<typename R::result_type>>
	class BinarySearchTree
	{
		public:
		typedef bst::Node<typename R::result_type, V> node_t;
		typedef typename node_t::node_ptr node_ptr;
		public:
		typedef const K& key_t;

		public:
			void put(key_t, const V&);
			V& get(key_t);
			bool contains(key_t) const;
		private:
			node_ptr root_ = nullptr;
			R op_;
	};

	template<typename K, typename V, typename R, typename S>
	void BinarySearchTree<K, V, R, S>::put(key_t k, const V& v)
	{
		root_ = bst::insert_recursive(root_, op_(k), v, S(), 
			[](key_t k, const V& v, node_ptr p) { return new node_t(k, v, p); });
	}

	template<typename K, typename V, typename R, typename S>
	V& BinarySearchTree<K, V, R, S>::get(key_t k)
	{
		node_ptr n = bst::find_recursive(root_, op_(k), S());

		if(!static_cast<bool>(n)) throw std::string("ERR: unknown key!");

		return n->value();
	}

	template<typename K, typename V, typename R, typename S>
	bool BinarySearchTree<K, V, R, S>::contains(key_t k) const
	{
		return static_cast<bool>(bst::find_recursive(root_, op_(k), S()));
	}
}

#endif

