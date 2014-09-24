#ifndef VM_KEYBOARD_H
#define VM_KEYBOARD_H

#include "ddef.h"
#include "Periphery.h"

namespace vm
{
	namespace periphery
	{
		class BasicKeyboard : public Periphery
		{
			public:
				BasicKeyboard( );
				void in(WORD);
				WORD out( );
				void step( );
			protected:
			private:
				bool enabled_;
		};
	}
}

#endif

