#include <iostream>
#include <cassert>
#include "Password.h"

#define MXT_PASSSYM ((char)7)
#define MXT_PASSPROMPT "Password:"

struct Password::Impl
{
	std::string pwd_;
};

void Password::i_keyPress(Controls c, const modifier_t& m)
{
	setString(impl_->pwd_);
	Input::i_keyPress(c, m);
	impl_->pwd_ = getContent();
	setString(std::string(impl_->pwd_.length(), MXT_PASSSYM));
}

void Password::i_setScreen(int w, int h)
{
	Input::i_setScreen(w, h);
	setPrompt(MXT_PASSPROMPT);
}

Password::Password(void) : impl_(new Impl)
{
}

Password::~Password(void) noexcept
{
	delete impl_;
}

