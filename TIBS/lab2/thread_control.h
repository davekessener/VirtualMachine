#ifndef DAV_BS_LAB2_THREADCONTROL
#define DAV_BS_LAB2_THREADCONTROL

#include <pthread.h>

#define MXT_TCTRL_UNLOCKED 0
#define MXT_TCTRL_LOCKED 1
#define MXT_TCTRL_QUIT 2

typedef struct
{
	pthread_t thread;
	pthread_mutex_t mtx;
	pthread_cond_t cond;
	int locked_status;
} TCTRL;

void TCTRL_new(TCTRL *, int, void *(*)(void *), void *);
void TCTRL_check(TCTRL *);
void TCTRL_toggle(TCTRL *);
void TCTRL_disable(TCTRL *);
void TCTRL_sleep(TCTRL *, int);
void TCTRL_delete(TCTRL *);

#endif

