#include "BigUInt.h"
#include <cassert>

namespace lisp
{
	BigUInt::BigUInt(const std::string& str) : blocks_{0}
	{
		for(const char& c : str)
		{
			mul(block_t(10));
			add(block_t(c - '0'));
		}
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
		if(length() == 1 && n.length() == 1)
		{
			block_t b = at(0) + n.at(0);
			if(b >= at(0))
			{
				at(0) = b;
				return;
			}
		}

		BigUInt r;
		int l = std::max(length(), n.length());

		r.resize(l);

		const BigUInt *pl = length() < n.length() ? &n : this;
		const BigUInt *ps = length() < n.length() ? this : &n;

		int i = 0;
		bool cIn = false, cOut = false;

		for(int ml = std::min(length(), n.length()) ; i < ml ; ++i)
		{
			block_t t = pl->at(i) + ps->at(i);

			cOut = t < pl->at(i);

			if(cIn)
			{
				if(!++t) cOut = true;
			}

			cIn = cOut;
			r.at(i) = t;
		}

		for(; i < l ; ++i)
		{
			r.at(i) = pl->at(i);
			if(cIn)
			{
				cIn = !++r.at(i);
			}
		}

		if(cIn)
		{
			r.resize(l + 1);
			r.at(l) = 1;
		}

		swap(r);
	}

	void BigUInt::sub(const BigUInt& n)
	{
		if(!gt(n))
		{
			BigUInt().swap(*this);
		}
		else if(length() == 1 && n.length() == 1)
		{
			at(0) -= n.at(0);
		}
		else
		{
			int i = 0;
			int l = n.length();
			bool cIn = false, cOut = false;
			BigUInt r;

			r.resize(l);

			for(; i < l ; ++i)
			{
				block_t b = at(i) - n.at(i);

				cOut = n.at(i) > at(i);
				
				if(cIn)
				{
					if(!b--) cOut = true;
				}

				cIn = cOut;
				r.at(i) = b;
			}

			for(l = length() ; i < l ; ++i)
			{
				r.at(i) = at(i);
				if(cIn)
				{
					cIn = !r.at(i)--;
				}
			}

			assert(!cIn);

			r.resize();

			swap(r);
		}
	}

	BigUInt::block_t BigUInt::getShiftedBlock(size_t x, size_t y) const
	{
		block_t p1((!x || !y) ? 0 : (at(x - 1) >> (N - y)));
		block_t p2(x == blocks_.size() ? 0 : (at(x) << y));
		return p1 | p2;
	}

	void BigUInt::mul(const BigUInt& n)
	{
		if(zero() || n.zero()) { swap(BigUInt()); return; }
		if(length() == 1 && n.length() == 1)
		{
			block_t b = at(0) * n.at(0);
			if(b >= at(0) && b >= n.at(0))
			{
				at(0) = b;
				return;
			}
		}

		BigUInt r;

		r.resize(blocks_.size() + n.blocks_.size());

		for(size_t i = 0 ; i < blocks_.size() ; ++i)
		{
			for(size_t j = 0 ; j < N ; ++j)
			{
				if(!(at(i) & (block_t(1) << j))) continue;

				bool cIn = false, cOut = false;
				size_t k2 = i;
				for(size_t k1 = 0 ; k1 <= n.blocks_.size() ; ++k1, ++k2)
				{
					block_t t(r.at(k2) + n.getShiftedBlock(k1, j));
					cOut = t < r.at(k2);
					if(cIn)
					{
						if(!++t) cOut = true;
					}
					r.at(k2) = t;
					cIn = cOut;
				}

				while(cIn)
				{
					cIn = !++r.at(k2++);
				}
			}
		}

		while(!r.blocks_.back()) r.blocks_.pop_back();

		swap(r);
	}

	BigUInt BigUInt::div(const BigUInt& n)
	{
		if(n.zero()) throw std::string("Divide by zero error");

		if(zero()) return BigUInt(0);

		if(lt(n))
		{
			BigUInt t;
			swap(t);
			return t;
		}

		if(length() == 1)
		{ 
			BigUInt t(at(0) % n.at(0));
			at(0) /= n.at(0);
			return t;
		}

		
	size_t i, j, k;
	unsigned int i2;
	block_t temp;
	bool borrowIn, borrowOut;

	size_t origLen = len;

	allocateAndCopy(len + 1);
	len++;
	blk[origLen] = 0;

	Blk *subtractBuf = new Blk[len];

	q.len = origLen - b.len + 1;
	q.allocate(q.len);

	for (i = 0; i < q.len; i++)
		q.blk[i] = 0;

	i = q.len;
	while (i > 0) {
		i--;
		q.blk[i] = 0;
		i2 = N;
		while (i2 > 0) {
			i2--;
			for (j = 0, k = i, borrowIn = false; j <= b.len; j++, k++) {
				temp = blk[k] - getShiftedBlock(b, j, i2);
				borrowOut = (temp > blk[k]);
				if (borrowIn) {
					borrowOut |= (temp == 0);
					temp--;
				}
				subtractBuf[k] = temp; 
				borrowIn = borrowOut;
			}
			for (; k < origLen && borrowIn; k++) {
				borrowIn = (blk[k] == 0);
				subtractBuf[k] = blk[k] - 1;
			}
			if (!borrowIn) {
				q.blk[i] |= (Blk(1) << i2);
				while (k > i) {
					k--;
					blk[k] = subtractBuf[k];
				}
			} 
		}
	}
	if (q.blk[q.len - 1] == 0)
		q.len--;
	zapLeadingZeros();
	delete [] subtractBuf;

		
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
		return length() == 1 && at(0) == 0;
	}

	bool BigUInt::eq(const BigUInt& n) const
	{
		if(length() != n.length()) return false;

		for(int i = 0, l = length() ; i < l ; ++i)
		{
			if(at(i) != n.at(i)) return false;
		}

		return true;
	}

	bool BigUInt::gt(const BigUInt& n) const
	{
		if(length() != n.length()) return length() > n.length();

		for(int i = length() - 1 ; i >= 0 ; --i)
		{
			if(at(i) != n.at(i))
			{
				return at(i) > n.at(i);
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
		return at(0);
	}

	const std::string BigUInt::toHex(void) const
	{
		if(zero()) return "0";

		std::stack<char> s;
		int nc = 0;

		static const int c = sizeof(block_t) << 1;
		static const char *d = "0123456789abcdef";

		for(size_t i = 0 ; i < length() ; ++i)
		{
			block_t b = at(i);

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

			if(i < length() - 1) while(nc) { s.push('0'); nc--; }
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

