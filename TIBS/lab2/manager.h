#ifndef DAV_BS_LAB2_MANAGER
#define DAV_BS_LAB2_MANAGER

#include <semaphore.h>
#include "thread_control.h"

#define MXT_TMANAGER_STOPPED 0
#define MXT_TMANAGER_RUNNING 1

typedef unsigned int uint;
typedef uint tid_t;

typedef struct
{
	TCTRL **threads;
	uint c, i;
	struct
	{
		uint flag;
		sem_t lock;
	} running;
} TMANAGER;

typedef void (* thread_core_t)(TMANAGER *, tid_t, TCTRL *, void *);

void TMANAGER_new(TMANAGER *);
tid_t TMANAGER_run(TMANAGER *, thread_core_t, void *);
uint TMANAGER_running(TMANAGER *);
uint TMANAGER_status(TMANAGER *, tid_t);
void TMANAGER_toggle(TMANAGER *, tid_t);
void TMANAGER_wait(TMANAGER *);
void TMANAGER_shutdown(TMANAGER *);
void TMANAGER_delete(TMANAGER *);

#endif

