#ifndef NBTEDIT_OBJECT_H
#define NBTEDIT_OBJECT_H

#include <string>
#include <memory>
#include "Node.h"

class Object
{
	public:
		Object( );
		Node_ptr get( ) { return node_; }
		void load(const std::string&);
		void save(const std::string& = "");
		const std::string& filename( ) const { return fn_; }
	private:
		std::string fn_;
		Node_ptr node_;
};

typedef std::shared_ptr<Object> Object_ptr;

#endif

