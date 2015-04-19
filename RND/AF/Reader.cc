#include <iterator>
#include <map>
#include <dav/io.hpp>
#include "Reader.h"

#define MXT_STR_DEA "^"
#define MXT_STR_RULE ":"
#define MXT_STR_INITIAL "*"
#define MXT_STR_ACCEPTING "%"
#define MXT_STR_TARGET ">"
#define MXT_STR_SYMBOL "|"
#define MXT_STR_END "<"

template<typename T, typename ... TT>
std::unique_ptr<T> make_unique(TT&& ... args)
{
	return std::unique_ptr<T>(new T(std::forward<TT>(args) ...));
}

namespace AF { namespace DEA {

using Cmd_t = std::string;

class Reader::Impl
{
	using In_t = dav::io::ReadContainer<Cmd_t>;

	public:
		Impl( );
		DEA generate(In_t&&);
	private:
		void read_DEA(In_t&);
		void read_Rule(In_t&);
		void read_Initial(In_t&);
		void read_Accepting(In_t&);
		void read_Recipient(In_t&);
		void read_Symbol(In_t&);
		void read_End(In_t&);
	private:
		Builder buf_;
		State from_, to_;
		std::map<std::string, void (Impl::*)(In_t&)> cmds_;
};

DEA Reader::generate(std::istream& in)
{
	return impl_->generate(dav::io::IStreamContainer<Cmd_t>(in));
}

// # ===========================================================================

Reader::Impl::Impl(void)
{
	cmds_[MXT_STR_DEA]       = &Impl::read_DEA;
	cmds_[MXT_STR_RULE]      = &Impl::read_Rule;
	cmds_[MXT_STR_INITIAL]   = &Impl::read_Initial;
	cmds_[MXT_STR_ACCEPTING] = &Impl::read_Accepting;
	cmds_[MXT_STR_TARGET]    = &Impl::read_Recipient;
	cmds_[MXT_STR_SYMBOL]    = &Impl::read_Symbol;
	cmds_[MXT_STR_END]       = &Impl::read_End;
}

DEA Reader::Impl::generate(In_t&& reader)
{
	buf_ = Builder{};
	from_ = to_ = State{};

	try
	{
		read_DEA(reader);
	}
	catch(const std::exception& e)
	{
		throw std::string("ERR: Some error reading DEA: '") + e.what() + "'!";
	}

	return std::move(buf_).generate();
}

void Reader::Impl::read_DEA(In_t& reader)
{
	assert(reader.top() == MXT_STR_DEA);
	reader.pop();

	read_Rule(reader);
}

void Reader::Impl::read_Rule(In_t& reader)
{
	assert(reader.top() == MXT_STR_RULE);
	reader.pop();

	from_ = reader.top(); reader.pop();

	(this->*cmds_[reader.top()])(reader);
}

void Reader::Impl::read_Initial(In_t& reader)
{
	assert(reader.top() == MXT_STR_INITIAL);
	reader.pop();

	buf_.setInitialState(from_);

	read_Recipient(reader);
}

void Reader::Impl::read_Accepting(In_t& reader)
{
	assert(reader.top() == MXT_STR_ACCEPTING);
	reader.pop();

	buf_.makeStateAccepting(from_);

	read_Recipient(reader);
}

void Reader::Impl::read_Recipient(In_t& reader)
{
	assert(reader.top() == MXT_STR_TARGET);
	reader.pop();

	to_ = reader.top(); reader.pop();

	read_Symbol(reader);
}

void Reader::Impl::read_Symbol(In_t& reader)
{
	assert(reader.top() == MXT_STR_SYMBOL);
	reader.pop();

	buf_.addRelation(from_, reader.top(), to_); reader.pop();

	(this->*cmds_[reader.top()])(reader);
}

void Reader::Impl::read_End(In_t& reader)
{
	assert(reader.top() == MXT_STR_END);
	reader.pop();
}

// # ===========================================================================

Reader::Reader(void) : impl_(make_unique<Impl>())
{
}

Reader::~Reader(void) = default;

} }

