#ifndef NBTEDIT_NODE_H
#define NBTEDIT_NODE_H

#include <string>
#include <memory>
#include <cassert>

#define MXT_DAVVEC

#ifdef MXT_DAVVEC
#include <dav/stable_vector.hpp>
#define MXT_VECTOR(t) dav::stable_vector<t>
#else
#include <vector>
#define MXT_VECTOR(t) std::vector<t>
#endif

class Node : public std::enable_shared_from_this<Node>
{
	public:
	typedef std::shared_ptr<Node> Node_ptr;
	typedef std::weak_ptr<Node> Node_wptr;
	typedef MXT_VECTOR(Node_ptr) vec_t;
	typedef vec_t::iterator iterator;
	typedef vec_t::const_iterator const_iterator;
	typedef unsigned int uint;

	public:
		Node( );
		virtual ~Node( ) { }
		void open( );
		void close( );
		void addChild(Node_ptr p);
		std::string getContent( );
		void erase(bool = false);
		void erase(iterator, bool = false);
		void next( );
		void prev( );
		inline void toggle( ) { setOpen(!opened_); }
		inline void setOpen(bool f) { (this->*(f ? &Node::open : &Node::close))(); }
		inline bool isOpen( ) const { return opened_; }
		inline void setParent(Node_ptr p) { parent_ = p; }
		inline bool hasParent( ) const { return static_cast<bool>(parent_.lock()); }
		inline Node_ptr parent( ) { return parent_.lock(); }
		inline Node_ptr current( ) { assert(hasChildren()); return vec_.at(idx_); }
		inline size_t size( ) const { return vec_.size(); }
		inline bool empty( ) const { return vec_.empty(); }
		inline bool hasChildren( ) const { return !vec_.empty(); }
		inline int index( ) { return idx_; }
		inline iterator begin( ) { return vec_.begin(); }
		inline iterator end( ) { return vec_.end(); }
		inline const_iterator begin( ) const { return vec_.cbegin(); }
		inline const_iterator end( ) const { return vec_.cend(); }
		inline const_iterator cbegin( ) const { return vec_.cbegin(); }
		inline const_iterator cend( ) const { return vec_.cend(); }
	protected:
		vec_t& getChildren( ) { return vec_; }
		void dirty( ) { dirty_ = true; }
	private:
		inline void processError( ) { if(errno_) i_doProcessError(errno_); errno_ = 0; }
	private:
		virtual void i_doOpen( ) { }
		virtual void i_doClose( ) { }
		virtual uint i_canErase(Node_ptr) const { return 0; }
		virtual void i_doErase(Node_ptr) { }
		virtual void i_doNext( ) { }
		virtual void i_doPrev( ) { }
		virtual void i_doProcessError(uint) const = 0;
		virtual std::string i_doGetContent( ) const = 0;
	private:
		Node_wptr parent_;
		bool opened_, dirty_;
		std::string content_;
		uint idx_, errno_;
		vec_t vec_;
};

typedef Node::Node_ptr Node_ptr;

#endif

