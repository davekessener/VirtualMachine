#include <malloc.h>
#include <pthread.h>
#include <signal.h>
#include "manager.h"
#include "system.h"

typedef struct
{
	TMANAGER *manager;
	thread_core_t core;
	tid_t id;
	TCTRL *ctrl;
	void *data;
} thread_args_t;

void *TMANAGER_runthread(void *);

// # ---------------------------------------------------------------------------

void TMANAGER_new(TMANAGER *this)
{
	this->threads = NULL;
	this->c = this->i = 0;

	sys_sem_init(&this->running.lock, 0);
	this->running.flag = 1;
}

tid_t TMANAGER_run(TMANAGER *this, thread_core_t core, void *data)
{
	tid_t id;
	thread_args_t *args;

	if(this->i == this->c)
	{
		this->threads = realloc(this->threads, (this->c = 2 * this->c + 1) * sizeof(*this->threads));
	}

	id = this->i++;
	this->threads[id] = malloc(sizeof(**this->threads));
	args = malloc(sizeof(*args));

	args->manager = this;
	args->core = core;
	args->id = id;
	args->ctrl = this->threads[id];
	args->data = data;

	TCTRL_new(this->threads[id], MXT_TCTRL_LOCKED, &TMANAGER_runthread, args);

	return id;
}

uint TMANAGER_running(TMANAGER *this)
{
	return this->running.flag;
}

uint TMANAGER_status(TMANAGER *this, tid_t id)
{
	return this->threads[id]->locked_status == MXT_TCTRL_UNLOCKED ? MXT_TMANAGER_RUNNING : MXT_TMANAGER_STOPPED;
}

void TMANAGER_toggle(TMANAGER *this, tid_t id)
{
	TCTRL_toggle(this->threads[id]);
}

void TMANAGER_wait(TMANAGER *this)
{
	int i;
	for(i = 0 ; i < this->i ; ++i)
	{
		TMANAGER_toggle(this, i);
	}

	sem_wait(&this->running.lock);

	this->running.flag = 0;

	for(i = 0 ; i < this->i ; ++i)
	{
		TCTRL_disable(this->threads[i]);
	}
}

void TMANAGER_shutdown(TMANAGER *this)
{
	sys_sem_post(&this->running.lock);
}

void TMANAGER_delete(TMANAGER *this)
{
	int i;
	for(i = 0 ; i < this->i ; ++i)
	{
		TCTRL_delete(this->threads[i]);
		free(this->threads[i]);
	}

	free(this->threads);
	this->threads = NULL;
	this->i = this->c = 0;
}

// # ---------------------------------------------------------------------------

void *TMANAGER_runthread(void *data)
{
	thread_args_t *args = (thread_args_t *) data;

	args->core(args->manager, args->id, args->ctrl, args->data);

	free(args);

	pthread_exit(NULL);
}

