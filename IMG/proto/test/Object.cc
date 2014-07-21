#include "Object.h"

using namespace std;

ostream& operator<<(ostream& s, const Object& o)
{
	return s << "Value: " << o._v << endl;
}

Object::Object(void) : _v(0)
{
	cout << "Object (default): " << _v << endl;
}

Object::Object(int v) : _v(v)
{
	cout << "Object (int): " << _v << endl;
}

Object::Object(const Object& o) : _v(o._v)
{
	cout << "Object (copy): " << _v << endl;
}

Object::~Object(void)
{
	cout << "Object (destr): " << _v << endl;
}

Object& Object::operator=(const Object& o)
{
	_v = o._v;
	cout << "Object =: " << _v << endl;
}

