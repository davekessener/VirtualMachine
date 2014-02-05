#include <iostream>
#include <vector>
#include <memory>
#include <map>

typedef unsigned char BYTE;

struct B
{
	virtual ~B() { }
	virtual BYTE getID() = 0;
};

template<BYTE>
struct C : public B
{
	BYTE getID() { return ID; }
	static const BYTE ID;
};

template<BYTE id>
const BYTE C<id>::ID = id;

template<BYTE ID>
class A : public C<ID>, private std::vector<std::string>
{
	typedef std::vector<std::string> v_t;
  	public:
		A() { init(); }
		void addS(const char *s) { v_t::push_back(std::string(s)); }
		std::string& getAt(int i) { return v_t::operator[](i); }
	private:
		void init();
};

#ifndef A_IMP
extern template class A<3>;
#endif

