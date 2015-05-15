#ifndef LISP_BIGUINT_H
#define LISP_BIGUINT_H

#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include "common.h"
#include "auto_ptr.hpp" 

namespace lisp
{
	class BigUInt
	{
		typedef unsigned long long block_t;
		static const size_t N = sizeof(block_t) << 3;

		public:
			explicit BigUInt(const std::string&);
			BigUInt(const BigUInt&);
			BigUInt(block_t = block_t());
			~BigUInt( );
			void swap(BigUInt&) throw();
			void swap(BigUInt&&) throw();
			void add(const BigUInt&);
			void sub(const BigUInt&);
			void mul(const BigUInt&);
			BigUInt div(const BigUInt&);
			inline void mod(const BigUInt& n) { swap(div(n)); }
			void exp(const BigUInt&);
			bool zero( ) const;
			bool eq(const BigUInt&) const;
			bool gt(const BigUInt&) const;
			inline bool lt(const BigUInt& n) const { return !gt(n) && !eq(n); }
			inline bool ne(const BigUInt& n) const { return !eq(n); }
			inline bool ge(const BigUInt& n) const { return gt(n) || eq(n); }
			inline bool le(const BigUInt& n) const { return !gt(n); }
			BigUInt& operator++( );
			BigUInt operator++(int);
			BigUInt& operator--( );
			BigUInt operator--(int);
			block_t value( ) const;
			const std::string toHex( ) const;
			std::ostream& print(std::ostream&) const;
		private:
			void resize(size_t);
			void resize( );
			inline size_t length( ) const { return blocks_.size(); }
			inline block_t& at(size_t i) { if(i >= length()) blocks_.resize(i + 1); return blocks_.at(i); }
			inline block_t at(size_t i) const { return blocks_.at(i); }
			block_t getShiftedBlock(size_t, size_t) const;
		private:
			std::vector<block_t> blocks_;
	};

	inline void swap(BigUInt& n1, BigUInt& n2) throw() { n1.swap(n2); }
	inline std::ostream& operator<<(std::ostream& os, const BigUInt& n) { return n.print(os); }
}

#endif

