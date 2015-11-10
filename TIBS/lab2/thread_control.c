#include <errno.h>
#include <signal.h>
#include "thread_control.h"
#include "system.h"

void TCTRL_new(TCTRL *this, int status, void *(*fn)(void *), void *d)
{
	sys_pthread_cond_init(&this->cond);
	sys_pthread_mutex_init(&this->mtx);
	this->locked_status = status;
	sys_pthread_create(&this->thread, fn, d);
}

void TCTRL_check(TCTRL *this)
{
	sys_pthread_mutex_lock(&this->mtx);
	while(this->locked_status == MXT_TCTRL_LOCKED)
	{
		sys_pthread_cond_wait(&this->cond, &this->mtx);
	}
	sys_pthread_mutex_unlock(&this->mtx);
}

void TCTRL_toggle(TCTRL *this)
{
	sys_pthread_mutex_lock(&this->mtx);
	if(this->locked_status != MXT_TCTRL_QUIT)
	{
		this->locked_status = !this->locked_status;
		if(this->locked_status == MXT_TCTRL_UNLOCKED)
		{
			sys_pthread_cond_broadcast(&this->cond);
		}
	}
	sys_pthread_mutex_unlock(&this->mtx);

	sys_pthread_kill(this->thread, SIGUSR1);
}

void TCTRL_disable(TCTRL *this)
{
	sys_pthread_mutex_lock(&this->mtx);
	this->locked_status = MXT_TCTRL_QUIT;
	sys_pthread_cond_broadcast(&this->cond);
	sys_pthread_mutex_unlock(&this->mtx);
}

void TCTRL_sleep(TCTRL *this, int t)
{
	sys_pthread_mutex_lock(&this->mtx);
	if(this->locked_status == MXT_TCTRL_UNLOCKED)
	{
		struct timespec ts;
		int r;
		sys_clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += t / 1000;
		ts.tv_nsec += (t % 1000) * 1000000L;
		ts.tv_sec += ts.tv_nsec / 1000000000L;
		ts.tv_nsec %= 1000000000L;

		while(this->locked_status == MXT_TCTRL_UNLOCKED)
		{
			if((r = pthread_cond_timedwait(&this->cond, &this->mtx, &ts)) == ETIMEDOUT ||
				r == EINVAL)
				break;
			else if(r)
				reportError("pthread_cond_timedwait", r);
		}
	}
	sys_pthread_mutex_unlock(&this->mtx);
}

void TCTRL_delete(TCTRL *this)
{
	pthread_kill(this->thread, SIGUSR1);
	sys_pthread_join(this->thread);
	sys_pthread_mutex_destroy(&this->mtx);
	sys_pthread_cond_destroy(&this->cond);
}

