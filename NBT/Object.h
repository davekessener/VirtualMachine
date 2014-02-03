#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>

class Object
{
	public:
		Object( );
		Object(int);
		Object(const Object&);
		~Object( );
		Object& operator=(const Object&);
		friend std::ostream& operator<<(std::ostream&, const Object&);
	private:
		int _v;
};

#endif

