#include <map>
#include "SymTable.h"

namespace vm { namespace assembler {

class SymTable::Impl
{
	typedef std::map<std::string, WORD> lbl_map;

	public:
		Impl( );
		~Impl( ) throw();
		inline void insert(const std::string& lbl, int p) { lbls_[lbl] = p; }
		inline void clear( ) { lbls_.clear(); }
	private:
		lbl_map lbls_;
};

// # ===========================================================================

SymTable::Impl::Impl(void)
{
}

SymTable::Impl::~Impl(void) throw()
{
}

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
	impl_->insert(lbl, pos);
}

void SymTable::clear(void)
{
	impl_->clear();
}

}}

