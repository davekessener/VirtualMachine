#ifndef SIU_ONEXIT_H
#define SIU_ONEXIT_H

#include <functional>

class OnExit
{
	public:
		OnExit(std::function<void(void)> fn) : _fn(fn) { }
		OnExit(const OnExit& oe) : _fn(oe._fn) { }
		~OnExit(void) { _fn(); }
	private:
		OnExit& operator=(const OnExit&);
		std::function<void(void)> _fn;
};

#endif

