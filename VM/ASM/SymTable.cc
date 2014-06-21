#include <map>
#include <algorithm>
#include "SymTable.h"

namespace vm { namespace assembler {

class SymTable::Impl
{
	typedef SymTable::lbl_map lbl_map;

	public:
		inline Impl( ) : offset_(0) { }
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

//WORD& SymTable::operator[](const std::string& id)
//{
//	return impl_->lbls_[id];
//}

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

SymTable::iterator SymTable::begin(void)
{
	return impl_->lbls_.begin();
}

SymTable::iterator SymTable::end(void)
{
	return impl_->lbls_.end();
}

SymTable::const_iterator SymTable::begin(void) const
{
	return impl_->lbls_.cbegin();
}

SymTable::const_iterator SymTable::end(void) const
{
	return impl_->lbls_.cend();
}

SymTable::const_iterator SymTable::cbegin(void) const
{
	return impl_->lbls_.cbegin();
}

SymTable::const_iterator SymTable::cend(void) const
{
	return impl_->lbls_.cend();
}

}}

