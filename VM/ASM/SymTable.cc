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

void SymTable::insert(const std::string& lbl, int pos)
{
	impl_->lbls_[lbl] = static_cast<WORD>(pos);
}

bool SymTable::knows(const std::string& lbl) const
{
	return std::find_if(impl_->lbls_.cbegin(), impl_->lbls_.cend(), 
			[lbl](const std::pair<std::string, WORD>& p) { return p.first == lbl; }) 
		!= impl_->lbls_.cend();
}

WORD SymTable::get(const std::string& lbl) const
{
	return impl_->lbls_.at(lbl);
}

void SymTable::clear(void)
{
	impl_->lbls_.clear();
}

}}

