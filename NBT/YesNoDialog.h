#ifndef CURSE_YESNODIALOG_H
#define CURSE_YESNODIALOG_H

#include <iostream>
#include <functional>
#include <cstring>
#include "Curse.h"

#ifdef CURSE_YESNODIALOG_MAIN
#endif

namespace ncurses
{
	enum class Choices : std::uint8_t
	{
		NONE,
		YES,
		NO
	};

	class YesNoDialog : public Curse
	{
		typedef std::function<void(bool)> answerFn;
		typedef std::function<void(bool)> drawFn;

		public:
			YesNoDialog(const std::string&, answerFn, Choices = Choices::NONE);
			~YesNoDialog( );
		protected:
			void init( );
			void input(int);
			void update(int);
			void refresh( );
			void finalize( );
		private:
			std::string msg;
			answerFn answer;
			drawFn drawYes, drawNo;
			Choices select;
			int ox, oy;

			static const char *YES;
			static const char *NO;
	};
}

#endif

