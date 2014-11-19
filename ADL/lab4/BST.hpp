#ifndef DAV_BST_H
#define DAV_BST_H

#include <type_traits>
#include <functional>
#include <iterator>
#include <cassert>
#include "STA.h"

#define MXT_MAX_STA_SIZE 64

namespace dav
{
	namespace bst
	{
		template<typename P, typename K, typename V, typename H, typename S, typename C>
		P insert(P n, K k, const V& v, H&& hash, S&& op, C&& create)
		{
			P p(nullptr), r(n);
			P *np(&n);
			auto key(hash(k));

			while(static_cast<bool>(*np))
			{
				bool f;

				p = *np;

				if(!(f = op((*np)->key(), key)) && !op(key, (*np)->key()))
				{
					(*np)->value() = v; break;
				}
				else
				{
					np = !f ? &(*np)->left() : &(*np)->right();
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
				n = !f ? n->left() : n->right();
			}

			return n;
		}

		template<typename P>
		P rotate(P p, std::function<P&(P)> left, std::function<P&(P)> right)
		{
			P n(right(p));
			right(p) = left(n);
			if(static_cast<bool>(left(n))) left(n)->parent() = p;
			left(n) = p;
			n->parent() = p->parent();
			p->parent() = n;

			return n;
		}

		template<typename P, typename K, typename V, typename H, typename S, typename C>
		P insertRoot(P n, K k, const V& v, H&& hash, S&& op, C&& create, P p = nullptr)
		{
			static std::function<P&(P)> left = [](P p) -> P& { return p->left(); };
			static std::function<P&(P)> right = [](P p) -> P& { return p->right(); };

			if(!static_cast<bool>(n))
			{
				return create(k, v, p);
			}
			else
			{
				bool f;
				auto key(hash(k));

				if(!(f = op(n->key(), key)) && !op(key, n->key()))
				{
					n->value() = v;
				}
				else if(!f)
				{
					n->left() = insertRoot(n->left(), k, v, hash, op, create, n);
					n = rotate(n, right, left);
				}
				else
				{
					n->right() = insertRoot(n->right(), k, v, hash, op, create, n);
					n = rotate(n, left, right);
				}

				return n;
			}
		}

//		template<typename P, typename K, typename V, typename S, typename C>
//		P insert_recursive(P n, K k, const V& v, S&& op, C&& create, P p = nullptr)
//		{
//			typedef typename std::decay<decltype(*n)>::type node_t;
//			bool f;
//			P& (node_t::*fn)();
//
//			return   static_cast<bool>(n)
//				   ? (   (
//				             (!(f = op(n->key(), k)) && !op(k, n->key()))
//				           ? (n->value() = v, n)
//				           : (   fn = f ? &node_t::left : &node_t::right
//				               , (n->*fn)() = insert_recursive((n->*fn)(), k, v, op, create, n))
//				         )
//				       , n)
//				   : create(k, v, p);
//		}
//
//		template<typename P, typename K, typename S>
//		P find_recursive(P n, K k, S&& op)
//		{
//			bool f;
//
//			return   static_cast<bool>(n) && ((f = op(n->key(), k)) || op(k, n->key()))
//				   ? find_recursive(f ? n->left() : n->right(), k, op)
//				   : n;
//		}

		template<typename T>
		struct Ident
		{
			typedef T result_type;

			const T& operator()(const T& t) const { return t; }
		};

		template<typename T>
		struct Deref
		{
			T t;

			typedef typename std::decay<decltype(*t)>::type type;
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

		template<typename P>
		size_t depth(P p)
		{
			if(!static_cast<bool>(p)) return 0;

			size_t l(depth(p->left())), r(depth(p->right()));

			return l > r ? l + 1 : r + 1;
		}

		template<typename H, typename K, typename V>
		class Node : public SmallType
		{
			typedef Node<H, K, V> self;
			public:
			typedef typename std::pair<K, V> pair_type;
			typedef typename H::result_type hash_type;
			typedef K key_type;
			typedef V value_type;
			typedef value_type *pointer;
			typedef const value_type *const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef std::size_t size_type;
			typedef std::ptrdiff_t difference_type;
//			typedef typename std::shared_ptr<self> node_ptr;
			typedef self *node_ptr;
			typedef const self *const_node_ptr;

			public:
				Node(const key_type& k, const_reference v, node_ptr p)
					: storage_(std::make_pair(k, v)), key_(H()(k)), parent_(p) { }
				~Node( ) { delete left_; delete right_; }
				inline const hash_type& key( ) const { return key_; }
				inline reference value( ) { return pair().second; }
				inline pair_type& pair( ) { return storage_; }
				inline node_ptr& left( ) { return left_; }
				inline node_ptr& right( ) { return right_; }
				inline node_ptr& parent( ) { return parent_; }
				inline const_reference value( ) const { return pair().second; }
				inline const pair_type& pair( ) const { return storage_; }
				inline const node_ptr& left( ) const { return left_; }
				inline const node_ptr& right( ) const { return right_; }
				inline const node_ptr& parent( ) const { return parent_; }
			private:
				pair_type storage_;
				hash_type key_;
				node_ptr left_ = nullptr, right_ = nullptr, parent_;
		};

		template<typename P>
		class BSTIter
		{
			typedef typename Deref<P>::type node_t;
			typedef BSTIter<P> self;
			typedef std::function<P(P)> fn_t;
			public:
			typedef typename node_t::pair_type value_type;
			typedef value_type *pointer;
			typedef const value_type *const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef typename node_t::size_type size_type;
			typedef typename node_t::difference_type difference_type;
			typedef std::bidirectional_iterator_tag iterator_category;

			private:
				P root_, p_;
			public:
				BSTIter(P p) : root_(p), p_(p)
					{ assert(static_cast<bool>(root_)); while(static_cast<bool>(p_->left())) p_ = p_->left(); }
				BSTIter(P p, P n) : root_(p), p_(n) { assert(static_cast<bool>(root_)); }
				auto operator*( ) -> decltype(p_->pair()) { assert(static_cast<bool>(p_)); return p_->pair(); }
				auto operator->( ) -> decltype(&**this) { assert(static_cast<bool>(p_)); return &**this; }
				self& operator++( )
				{
					if(static_cast<bool>(p_))
					{
						p_ = next(p_, (fn_t)([](P p) -> P { return p->left(); }), 
									  (fn_t)([](P p) -> P { return p->right(); }), 
									  (fn_t)([](P p) -> P { return p->parent(); }));
					}

					return *this;
				}
				self& operator--( )
				{
					if(static_cast<bool>(p_))
					{
						p_ = next(p_, (fn_t)([](P p) -> P { return p->right(); }), 
									  (fn_t)([](P p) -> P { return p->left(); }), 
									  (fn_t)([](P p) -> P { return p->parent(); }));
					}
					else
					{
						p_ = root_;
						while(static_cast<bool>(p_->right())) p_ = p_->right();
					}

					return *this;
				}
				self operator++(int) { self t(*this); ++*this; return t; }
				self operator--(int) { self t(*this); --*this; return t; }
				bool operator==(const self& i) const { return root_ == i.root_ && p_ == i.p_; }
				bool operator!=(const self& i) const { return !(i == *this); }
		};
	}

	template<typename K, typename V, typename R = bst::Ident<K>, typename S = std::less<typename R::result_type>>
	class BinarySearchTree
	{
		public:
		typedef bst::Node<R, K, V> node_t;
		typedef typename node_t::node_ptr node_ptr;
		typedef typename node_t::const_node_ptr const_node_ptr;
		public:
		typedef const K& key_type;
		typedef typename node_t::value_type value_type;
		typedef typename node_t::pointer pointer;
		typedef typename node_t::reference reference;
		typedef typename node_t::const_pointer const_pointer;
		typedef typename node_t::const_reference const_reference;
		typedef typename node_t::size_type size_type;
		typedef typename node_t::difference_type difference_type;
		typedef bst::BSTIter<node_ptr> iterator;
		typedef bst::BSTIter<const_node_ptr> const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		public:
			void insert(key_type, const V&);
			V& get(key_type);
			size_t count(key_type k) const { return contains(k); }
			bool contains(key_type) const;
			size_t depth( ) const { return depth_ ? depth_ : (empty() ? 0 : (depth_ = i_depth())); }
			bool empty( ) const { return !root_; }
			void clear( ) { delete root_; root_ = nullptr; depth_ = 0; }
			reference operator[](key_type k) { if(!contains(k)) insert(k, V()); return get(k); }
			iterator begin( ) { return iterator(root_); }
			iterator end( ) { return iterator(root_, nullptr); }
			const_iterator begin( ) const { return const_iterator(root_); }
			const_iterator end( ) const { return const_iterator(root_, nullptr); }
			const_iterator cbegin( ) const { return const_iterator(root_); }
			const_iterator cend( ) const { return const_iterator(root_, nullptr); }
			reverse_iterator rbegin( ) { return reverse_iterator(end()); }
			reverse_iterator rend( ) { return reverse_iterator(begin()); }
			const_reverse_iterator rbegin( ) const { return const_reverse_iterator(end()); }
			const_reverse_iterator rend( ) const { return const_reverse_iterator(begin()); }
			const_reverse_iterator crbegin( ) const { return const_reverse_iterator(end()); }
			const_reverse_iterator crend( ) const { return const_reverse_iterator(begin()); }
		private:
			size_t i_depth( ) const;
		private:
			node_ptr root_ = nullptr;
			mutable size_t depth_ = 0;
			R op_;
	};

	template<typename K, typename V>
	using RandomizedBinarySearchTree = BinarySearchTree<K, V, std::hash<K>>;

	template<typename K, typename V, typename R, typename S>
	void BinarySearchTree<K, V, R, S>::insert(key_type k, const V& v)
	{
		depth_ = 0;
		root_ = bst::insert(root_, k, v, op_, S(), 
			[](key_type k, const V& v, node_ptr p) { return new node_t(k, v, p); });
	}

	template<typename K, typename V, typename R, typename S>
	V& BinarySearchTree<K, V, R, S>::get(key_type k)
	{
		node_ptr n = bst::find(root_, op_(k), S());

		if(!static_cast<bool>(n)) throw std::string("ERR: unknown key!");

		return n->value();
	}

	template<typename K, typename V, typename R, typename S>
	bool BinarySearchTree<K, V, R, S>::contains(key_type k) const
	{
		return static_cast<bool>(bst::find(root_, op_(k), S()));
	}

	template<typename K, typename V, typename R, typename S>
	size_t BinarySearchTree<K, V, R, S>::i_depth(void) const
	{
		return bst::depth(root_);
	}
}

#endif

