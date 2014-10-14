#include "Evaluator.h"
#include "EvalStages.h"

namespace dav { namespace dil {

struct Evaluator::Impl
{
	Eval_ptr stage_;
	Impl( ) : stage_(new Stage0_Name) { }
};

// # ===========================================================================

void Evaluator::parse(const std::string& s)
{
	impl_->stage_->parse(s);

	if(impl_->stage_->hasNext())
	{
		impl_->stage_.reset(impl_->stage_->getNext());
	}
}

void Evaluator::out(std::ostream& os) const
{
	impl_->stage_->out(os);
}

// # ---------------------------------------------------------------------------

Evaluator::Evaluator(void) : impl_(new Impl)
{
}

Evaluator::Evaluator(const Evaluator& e) : impl_(e.impl_ ? new Impl(*e.impl_) : nullptr)
{
}

Evaluator::Evaluator(Evaluator&& ev) noexcept : impl_(nullptr)
{
	swap(ev);
}

Evaluator::~Evaluator(void) noexcept
{
	delete impl_;
}

Evaluator& Evaluator::operator=(const Evaluator& e)
{
	Evaluator t(e);
	swap(t);
	return *this;
}

Evaluator& Evaluator::operator=(Evaluator&& e) noexcept
{
	swap(e);
	return *this;
}

void Evaluator::swap(Evaluator& e) noexcept
{
	Impl *i = impl_;
	impl_ = e.impl_;
	e.impl_ = i;
}

}}

