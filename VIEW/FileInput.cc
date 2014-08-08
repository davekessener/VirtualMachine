#include <memory>
#include "FileInput.h"
#include "dir.h"

#define MXT_FILEPROMPT "Enter path:"

struct FileInput::Impl
{
	bool ready( ) const;
	std::string generate(const std::string&);
	std::string getNext( );
	void clear( );

	std::shared_ptr<dir> dir_;
};

bool FileInput::Impl::ready(void) const
{
	return static_cast<bool>(dir_);
}

std::string FileInput::Impl::generate(const std::string& s)
{
	dir_.reset(new dir(s));
	return dir_->getBase() + dir_->getBest();
}

std::string FileInput::Impl::getNext(void)
{
	return dir_->getBase() + dir_->getNext();
}

void FileInput::Impl::clear(void)
{
	dir_.reset();
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
		std::string s;
		if(!impl_->ready())
		{
			s = impl_->generate(getContent());
		}
		else
		{
			s = impl_->getNext();
		}
		setString(s);
		moveCursor(s.length());
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

