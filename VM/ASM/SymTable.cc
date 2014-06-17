#include <map>
#include <algorithm>
#include "SymTable.h"

namespace vm { namespace assembler {

class SymTable::Impl
{
	typedef std::map<std::string, WORD> lbl_map;

	public:
	private:
		friend class SymTable;
		lbl_map lbls_;
		int offset_;
};

// # ===========================================================================

// # ---------------------------------------------------------------------------

// # ===========================================================================

SymTable::SymTable(void) : impl_(new Impl)
{
}

SymTable::~SymTable(void) throw()
{
	delete impl_;
}

void SymTable::insert(const std::string& lbl, WORD pos)
{
	impl_->lbls_[lbl] = pos;
}

WORD& SymTable::operator[](const std::string& id)
{
	return impl_->lbls_[id];
}

bool SymTable::knows(const std::string& lbl) const
{
	return impl_->lbls_.count(lbl) > 0;
}

WORD SymTable::get(const std::string& lbl) const
{
	return impl_->lbls_.at(lbl) + impl_->offset_;
}

void SymTable::setOffset(int o)
{
	impl_->offset_ = o;
}

void SymTable::clear(void)
{
	impl_->lbls_.clear();
}

}}

