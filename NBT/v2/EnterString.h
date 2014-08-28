#ifndef NBTEDIT_ENTERSTRING_H
#define NBTEDIT_ENTERSTRING_H

#include <string>
#include <functional>
#include "Control.h"

class EnterString : public Control
{
	public:
	typedef std::function<void(const std::string&)> enter_fn;

	public:
		EnterString(int, int, enter_fn, const std::string& = "", const std::string& = "");
	private:
		void i_doRender( ) const;
		void i_doInput(int);
	private:
		void drawBorder( ) const;
	private:
		int x1_, y1_, x2_, y2_;
		size_t cpos_;
		enter_fn enter_;
		std::string input_;
		const std::string prompt_;
		std::vector<wchar_t> vert_, horz_;
};

#endif

