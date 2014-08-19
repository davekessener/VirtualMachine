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

class Node
{
	public:
	typedef std::shared_ptr<Node> Node_ptr;
	typedef MXT_VECTOR(Node_ptr) vec_t;
	typedef vec_t::iterator iterator;
	typedef vec_t::const_iterator const_iterator;

	public:
		Node( ) : opened_(false), dirty_(true) { }
		virtual ~Node( ) { }
		void open( ) { opened_ = true; i_doOpen(); }
		void close( ) { opened_ = false; i_doClose(); }
		void setOpen(bool f) { (this->*(f ? &Node::open : &Node::close))(); }
		void toggle( ) { setOpen(!opened_); }
		bool isOpen( ) const { return opened_; }
		iterator begin( ) { return vec_.begin(); }
		iterator end( ) { return vec_.end(); }
		const_iterator begin( ) const { return vec_.cbegin(); }
		const_iterator end( ) const { return vec_.cend(); }
		const_iterator cbegin( ) const { return vec_.cbegin(); }
		const_iterator cend( ) const { return vec_.cend(); }
		std::string getContent( ) { if(dirty_) { content_ = i_doGetContent(); dirty_ = false; } return content_; }
		size_t size( ) const { return vec_.size(); }
		bool empty( ) const { return vec_.empty(); }
		bool hasChildren( ) const { return !vec_.empty(); }
	protected:
		vec_t& getChildren( ) { return vec_; }
		void dirty( ) { dirty_ = true; }
	private:
		virtual void i_doOpen( ) { }
		virtual void i_doClose( ) { }
		virtual std::string i_doGetContent( ) const = 0;
	private:
		bool opened_, dirty_;
		std::string content_;
		vec_t vec_;
};

typedef Node::Node_ptr Node_ptr;

#endif

