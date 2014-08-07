#include <vector>
#include "FileInput.h"

#define MXT_FILEPROMPT "Enter path:"

struct FileInput::Impl
{
	bool ready( ) const;
	void generate(const std::string&);
	std::string getNext( );
	void clear( );

	std::string part_;
	std::vector<std::string> opt_;
	std::vector<std::string>::const_iterator cur_;
};

bool FileInput::Impl::ready(void) const
{
	return part_ != "";
}

void FileInput::Impl::generate(const std::string& s)
{
	part_ = s;
	std::string path = s.substr(0, s.find_last_of('/'));
	if(path.empty()) path = ".";
	// TODO
}

std::string FileInput::Impl::getNext(void)
{
	if(opt_.empty()) return part_;
	std::string r(*cur_);
	if(++cur_ == opt_.cend()) cur_ = opt_.cbegin();
	return r;
}

void FileInput::Impl::clear(void)
{
	part_ = "";
	std::vector<std::string>().swap(opt_);
	cur_ = opt_.cend();
}

void FileInput::i_setScreen(int w, int h)
{
	Input::i_setScreen(w, h);
	setPrompt(MXT_FILEPROMPT);
}

void FileInput::i_keyPress(Controls key, const modifier_t& m)
{
	if(key == Controls::TAB)
	{
		if(!impl_->ready()) impl_->generate(getContent());
		setString(impl_->getNext());
	}
	else
	{
		impl_->clear();
		Input::i_keyPress(key, m);
	}
}

FileInput::FileInput(void) : impl_(new Impl)
{
}

FileInput::~FileInput(void) noexcept
{
	delete impl_;
}

