#include "BigUInt.h"
#include <cassert>

namespace lisp
{
	BigUInt::BigUInt(void) : blocks_{0}
	{
	}

	BigUInt::BigUInt(const BigUInt& b) : blocks_(b.blocks_)
	{
	}

	BigUInt::BigUInt(block_t b) : blocks_{b}
	{
	}

	BigUInt::~BigUInt(void)
	{
	}

	void BigUInt::swap(BigUInt& b) throw()
	{
		blocks_.swap(b.blocks_);
	}

	void BigUInt::swap(BigUInt&& n) throw()
	{
		blocks_.swap(n.blocks_);
	}

	void BigUInt::add(const BigUInt& n)
	{
		BigUInt r;
		int l = std::max(length(), n.length());

		r.resize(l);

		const BigUInt *pl = length() < n.length() ? &n : this;
		const BigUInt *ps = length() < n.length() ? this : &n;

		int i = 0;
		bool cin = false, cout = false;

		for(int ml = std::min(length(), n.length()) ; i < ml ; ++i)
		{
			block_t t = pl->blocks_.at(i) + ps->blocks_.at(i);

			cout = t < pl->blocks_.at(i);

			if(cin)
			{
				if(!++t) cout = true;
			}

			cin = cout;
			r.blocks_.at(i) = t;
		}

		for(; i < l ; ++i)
		{
			r.blocks_.at(i) = pl->blocks_.at(i);
			if(cin)
			{
				cin = !++r.blocks_.at(i);
			}
		}

		if(cin)
		{
			r.blocks_.resize(l + 1);
			r.blocks_.at(l) = 1;
		}

		swap(r);
	}

	void BigUInt::sub(const BigUInt& n)
	{
		if(!gt(n))
		{
			BigUInt().swap(*this);
		}
		else
		{
			int i = 0;
			int l = n.length();
			bool cin = false, cout = false;
			BigUInt r;

			r.resize(l);

			for(; i < l ; ++i)
			{
				block_t b = blocks_.at(i) - n.blocks_.at(i);

				cout = n.blocks_.at(i) > blocks_.at(i);
				
				if(cin)
				{
					if(!b--) cout = true;
				}

				cin = cout;
				r.blocks_.at(i) = b;
			}

			for(l = length() ; i < l ; ++i)
			{
				r.blocks_.at(i) = blocks_.at(i);
				if(cin)
				{
					cin = !r.blocks_.at(i)--;
				}
			}

			assert(!cin);

			r.resize();

			swap(r);
		}
	}

	void BigUInt::mul(const BigUInt& n)
	{
		BigUInt r;

		if(!zero())
		{
			r = n;
			--*this;
			while(!zero())
			{
				r.add(n);
				--*this;
			}
		}

		swap(r);
	}

	BigUInt BigUInt::div(const BigUInt& n)
	{
		BigUInt c;

		assert(!n.zero());

		while(ge(n))
		{
			++c;
			sub(n);
		}

		swap(c);

		return c;
	}

	void BigUInt::exp(const BigUInt& n)
	{
		if(n.zero())
		{
			BigUInt(1).swap(*this);
			return;
		}

		BigUInt m(*this);

		for(BigUInt i(n) ; !i.zero() ; --i)
		{
			mul(m);
		}
	}

	bool BigUInt::zero(void) const
	{
		return length() == 1 && blocks_.at(0) == 0;
	}

	bool BigUInt::eq(const BigUInt& n) const
	{
		if(length() != n.length()) return false;

		for(int i = 0, l = length() ; i < l ; ++i)
		{
			if(blocks_.at(i) != n.blocks_.at(i)) return false;
		}

		return true;
	}

	bool BigUInt::gt(const BigUInt& n) const
	{
		if(length() != n.length()) return length() > n.length();

		for(int i = length() - 1 ; i >= 0 ; --i)
		{
			if(blocks_.at(i) != n.blocks_.at(i))
			{
				return blocks_.at(i) > n.blocks_.at(i);
			}
		}

		return false;
	}

	BigUInt& BigUInt::operator++(void)
	{
		add(1);
		return *this;
	}

	BigUInt BigUInt::operator++(int)
	{
		BigUInt r(*this);
		++*this;
		return r;
	}

	BigUInt& BigUInt::operator--(void)
	{
		sub(1);
		return *this;
	}

	BigUInt BigUInt::operator--(int)
	{
		BigUInt r(*this);
		--*this;
		return r;
	}

	BigUInt::block_t BigUInt::value(void) const
	{
		assert(length()==1);
		return blocks_.at(0);
	}

	const std::string BigUInt::toHex(void) const
	{
		if(zero()) return "0";

		std::stack<char> s;
		int nc = 0;

		static const int c = sizeof(block_t) << 1;
		static const char *d = "0123456789abcdef";

		for(int i = length() - 1 ; i >= 0 ; --i)
		{
			block_t b = blocks_.at(i);

			for(int j = 0 ; j < c ; ++j)
			{
				int t = static_cast<int>((b >> (j * 4)) & 0xf);

				if(t == 0)
				{
					nc++;
				}
				else
				{
					while(nc) { s.push('0'); nc--; }
					s.push(d[t]);
				}
			}
		}

		auto_ptr<char> p(new char[s.size() + 1]);
		char *t = p;

		while(!s.empty())
		{
			*t++ = s.top();
			s.pop();
		}
		*t = '\0';

		return std::string(static_cast<char *>(p), t);
	}

	std::ostream& BigUInt::print(std::ostream& os) const
	{
		return os << toHex();
	}

// # ---------------------------------------------------------------------------

	void BigUInt::resize(size_t l)
	{
		assert(l>0);
		blocks_.resize(l);
	}

	void BigUInt::resize(void)
	{
		size_t l = 1;
		for(size_t i = 0 ; i < blocks_.size() ; ++i)
		{
			if(blocks_.at(i) > 0) l = i + 1;
		}
		if(blocks_.size() > l) blocks_.resize(l);
	}
}

