#include "Node.h"

Node::Node(void) : parent_(), opened_(false), dirty_(true), idx_(0)
{
}

void Node::open(void)
{
	opened_ = true;
	i_doOpen();
}

void Node::close(void)
{
	opened_ = false;
	
	for(Node_ptr p : vec_)
	{
		p->close();
	}

	i_doClose();
	
	idx_ = 0;
}

void Node::addChild(Node_ptr p)
{
	vec_.insert(vec_.begin() + idx_, p);
	p->setParent(shared_from_this());
}

std::string Node::getContent(void)
{
	if(dirty_)
	{
		content_ = i_doGetContent();
		dirty_ = false;
	}
	
	return content_;
}

void Node::erase(iterator i, bool force)
{
	if(force || !(errno_ = i_canErase(*i)))
	{
		i_doErase(*i);
		vec_.erase(i);
		dirty();
	}
	else
	{
		processError();
	}
}

void Node::erase(bool force)
{
	assert(hasChildren());
	
	erase(vec_.begin() + idx_, force);

	if(!vec_.empty())
	{
		while(idx_ >= vec_.size()) --idx_;
	}
}

void Node::next(void)
{
	if(idx_ < vec_.size() - 1)
	{
		++idx_;
		i_doNext();
	}
}

void Node::prev(void)
{
	if(idx_ > 0)
	{
		--idx_;
		i_doPrev();
	}
}

