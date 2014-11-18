#ifndef DAV_BST_H
#define DAV_BST_H

#include <memory>
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

		template<typename K, typename V>
		class Node : public SmallType
		{
			typedef typename Wrapper<K>::type key_type;
			typedef typename Wrapper<V>::type value_type;
			typedef Node<K, V> self;
			public:
			typedef typename std::shared_ptr<self> node_ptr;

			public:
				Node(const K& k, const V& v) : key_(k), value_(v) { }
				const K& key( ) { return key_; }
				V& value( ) { return value_; }
				node_ptr& left( ) { return left_; }
				node_ptr& right( ) { return right_; }
			private:
				key_type key_;
				value_type value_;
				node_ptr left_, right_;
		};

		template<typename P, typename K, typename V, typename S>
		P insert(P n, K k, const V& v, S op)
		{
			if(!static_cast<bool>(n))
			{
				return std::make_shared<typename P::element_type>(k, v);
			}
			else
			{
				bool f;

				if(!(f = op(n->key(), k)) && !op(k, n->key()))
				{
					n->value() = v;
				}
				else if(f)
				{
					n->left() = insert(n->left(), k, v, op);
				}
				else
				{
					n->right() = insert(n->right(), k, v, op);
				}
	
				return n;
			}
		}
	
		template<typename P, typename K, typename S>
		P find(P n, K k, S op)
		{
			if(static_cast<bool>(n) && (op(n->key(), k) || op(k, n->key())))
			{
				if(op(n->key(), k))
				{
					return find(n->left(), k, op);
				}
				else
				{
					return find(n->right(), k, op);
				}
			}
			else
			{
				return n;
			}
		}

		template<typename T>
		struct Ident
		{
			typedef T result_type;

			const T& operator()(const T& t) const { return t; }
		};
	}

	template<typename K, typename V, typename R = bst::Ident<K>, typename S = std::less<typename R::result_type>>
	class BinarySearchTree
	{
		typedef bst::Node<typename R::result_type, V> node_t;
		typedef typename node_t::node_ptr node_ptr;
		public:
		typedef const K& key_t;

		public:
			void put(key_t, const V&);
			V& get(key_t);
			bool contains(key_t) const;
		private:
			node_ptr root_;
			R op_;
	};

	template<typename K, typename V, typename R, typename S>
	void BinarySearchTree<K, V, R, S>::put(key_t k, const V& v)
	{
		root_ = bst::insert(root_, op_(k), v, S());
	}

	template<typename K, typename V, typename R, typename S>
	V& BinarySearchTree<K, V, R, S>::get(key_t k)
	{
		node_ptr n = bst::find(root_, op_(k), S());

		if(!static_cast<bool>(n)) throw std::string("ERR: unknown key!");

		return n->value();
	}

	template<typename K, typename V, typename R, typename S>
	bool BinarySearchTree<K, V, R, S>::contains(key_t k) const
	{
		return static_cast<bool>(bst::find(root_, op_(k), S()));
	}
}

#endif

