#ifndef INPUT_H
#define INPUT_H

#include "Screen.h"

class Input : public Screen
{
	protected:
	using Screen::Controls;
	using Screen::modifier_t;
	typedef std::function<void(const std::string&)> activate_fn;

	public:
		Input( );
		virtual ~Input( ) noexcept;
		void setPrompt(const std::string&);
		void setActivate(activate_fn);
		const std::string& getContent( ) const;
	protected:
		void setString(const std::string&, int = -1, int = -1);
		void moveCursor(int);
		virtual void i_keyPress(Controls, const modifier_t&);
		virtual void i_setScreen(int, int);
	private:
		void i_suspend(bool);
	private:
		struct Impl;
		Impl *impl_;
};

#endif

