#ifndef NBTEDIT_STATUSBAR_H
#define NBTEDIT_STATUSBAR_H

#include "Object.h"

class StatusBar
{
	public:
		StatusBar(Object_ptr);
		std::string getStatus( );
	private:
		Object_ptr obj_;
};

#endif

