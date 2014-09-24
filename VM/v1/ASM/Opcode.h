#ifndef VM_ASM_OPCODE_H
#define VM_ASM_OPCODE_H

#include <iosfwd>
#include <vector>
#include "adef.h"

namespace vm
{
	namespace assembler
	{
		struct SymTable;

		class op_proxy
		{
			public:
			typedef std::vector<WORD> buf_vec;

			public:
				inline op_proxy& operator<<(WORD v) { buf_.push_back(v); return *this; }
				inline op_proxy& operator<<(const std::vector<WORD>& v) { buf_.insert(buf_.end(), v.cbegin(), v.cend()); return *this; }
				inline buf_vec& insert(buf_vec& v) const { v.insert(v.end(), buf_.begin(), buf_.end()); return v;}
			private:
				buf_vec buf_;
		};

		class Opcode
		{
			public:
				virtual ~Opcode( ) throw() { }
				virtual void out(std::ostream&) = 0;
				virtual int size( ) const = 0;
				virtual const op_proxy evaluate(SymTable&, int) const = 0;
				static Opcode *read(std::istream&);
			private:
		};

		inline std::ostream& operator<<(std::ostream& os, Opcode& op)
		{
			op.out(os);

			return os;
		}

		inline op_proxy::buf_vec operator<<(op_proxy::buf_vec& vec, const op_proxy& proxy)
		{
			return proxy.insert(vec);
		}
	}
}

#endif

