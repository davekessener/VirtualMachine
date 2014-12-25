#ifndef DAV_HUFFTREE_H
#define DAV_HUFFTREE_H

#include <string>
#include <set>
#include <map>
#include <memory>
#include <sstream>
#include "CountElems.hpp"

#define MXT_INTERNAL '*'

template<typename T>
struct CmpSec
{
	bool operator()(const T& t1, const T& t2) const { return t1.second <= t2.second; }
};

template<typename T>
class node_t
{
	typedef node_t<T> self;
	public:
	typedef std::shared_ptr<self> node_ptr;

	public:
		node_t(char c, node_ptr u = node_ptr(), node_ptr v = node_ptr())
			: c_(c), left_(u), right_(v)
		{
			evalInt();
		}
		template<typename I>
		std::string encode(I i1, I i2) const
		{
			std::stringstream ss;
			while(i1 != i2)
			{
				ss << translate(*i1, "");
				++i1;
			}
			return ss.str();
		}
		template<typename I>
		std::string decode(I i1, I i2) const
		{
			std::stringstream ss;
			const self *p = this;
			while(i1 != i2)
			{
				if(*i1 == '0' && static_cast<bool>(p->left_)) p = &*p->left_;
				else if(*i1 == '1' && static_cast<bool>(p->right_)) p = &*p->right_;
				else if(p->internal()) throw std::string("ERR: Ended on internal node!");
				else { ss << p->c_; p = this; }
				if(p != this) ++i1;
			}
			if(p->internal()) throw std::string("ERR: Ended on internal node!");
			ss << p->c_;
			return ss.str();
		}
		bool internal( ) const { return int_; }
		std::string toString( ) const
		{
			std::stringstream ss;
			ss << c_;
			if(static_cast<bool>(left_)) ss << left_->toString();
			if(static_cast<bool>(right_)) ss << right_->toString();
			return ss.str();
		}
	private:
		void evalInt( ) { int_ = static_cast<bool>(left_) || static_cast<bool>(right_); }
		std::string translate(char c, const std::string& p) const
		{
			if(c_ == c) return p;
			std::string r("");
			if(static_cast<bool>(left_)) r = left_->translate(c, p + "0");
			if(r.empty() && static_cast<bool>(right_)) r = right_->translate(c, p + "1");
			return r;
		}
	public:
		template<typename I>
		static node_ptr CreateTree(I i1, I i2)
		{
			typedef std::pair<node_ptr, int> wnode;
			std::map<char, int> d;
			countElements(i1, i2, d);
			std::set<wnode, CmpSec<wnode>> s;
			for(const auto& p : d)
			{
				s.insert(std::make_pair(std::make_shared<self>(p.first), p.second));
			}
			while(s.size() > 1)
			{
				wnode u(*s.begin()); s.erase(s.begin());
				wnode v(*s.begin()); s.erase(s.begin());
				s.insert(std::make_pair(std::make_shared<self>(MXT_INTERNAL, u.first, v.first), u.second + v.second));
			}
			return s.begin()->first;
		}
	private:
		char c_;
		node_ptr left_, right_;
		bool int_;
};

typedef node_t<char> Node;
typedef Node::node_ptr Node_ptr;

#endif

