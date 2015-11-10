#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "system.h"

typedef int (* QueueRoutine)(QUEUE *);

inline int call(QueueRoutine *rs, QUEUE *q)
{
	QueueRoutine r = rs[q->type];
	return r ? r(q) : MXT_QUEUE_ERR_OK;
}

typedef struct
{
	size_t count;
} Q_ST;

typedef struct
{
	sem_t semFill, semEmpty;
	pthread_mutex_t mtx;
} Q_SYNC;

// # ===========================================================================

int QR_new_ST(QUEUE *this)
{
	Q_ST *ctrl = malloc(sizeof(Q_ST));
	this->control = ctrl;

	ctrl->count = 0;

	return MXT_QUEUE_ERR_OK;
}

int QR_new_SYNC(QUEUE *this)
{
	Q_SYNC *ctrl = malloc(sizeof(Q_SYNC));
	this->control = ctrl;

	sys_pthread_mutex_init(&ctrl->mtx);
	sys_sem_init(&ctrl->semFill, this->capacity);
	sys_sem_init(&ctrl->semEmpty, 0);

	return MXT_QUEUE_ERR_OK;
}

QueueRoutine qrs_new[] = {
	&QR_new_ST,
	&QR_new_SYNC
};

// # ---------------------------------------------------------------------------

int QR_pre_push_ST(QUEUE *this)
{
	return ((Q_ST *) this->control)->count < this->capacity ? MXT_QUEUE_ERR_OK : MXT_QUEUE_ERR_FULL;
}

int QR_pre_push_SYNC(QUEUE *this)
{
	Q_SYNC *ctrl = (Q_SYNC *) this->control;
	
	if(sem_wait(&ctrl->semFill))
	{
		if(errno == EINTR)
			return MXT_QUEUE_ERR_SYNC;
		else
			reportError("sem_wait", errno);
	}

	sys_pthread_mutex_lock(&ctrl->mtx);

	return MXT_QUEUE_ERR_OK;
}

int QR_post_push_ST(QUEUE *this)
{
	++((Q_ST *) this->control)->count;

	return MXT_QUEUE_ERR_OK;
}

int QR_post_push_SYNC(QUEUE *this)
{
	Q_SYNC *ctrl = (Q_SYNC *) this->control;
	
	sys_sem_post(&ctrl->semEmpty);

	sys_pthread_mutex_unlock(&ctrl->mtx);

	return MXT_QUEUE_ERR_OK;
}

QueueRoutine qrs_pre_push[] = {
	&QR_pre_push_ST,
	&QR_pre_push_SYNC
};
QueueRoutine qrs_post_push[] = {
	&QR_post_push_ST,
	&QR_post_push_SYNC
};

// # ---------------------------------------------------------------------------

int QR_pre_poll_ST(QUEUE *this)
{
	return ((Q_ST *) this->control)->count ? MXT_QUEUE_ERR_OK : MXT_QUEUE_ERR_FULL;
}

int QR_pre_poll_SYNC(QUEUE *this)
{
	Q_SYNC *ctrl = (Q_SYNC *) this->control;
	
	if(sem_wait(&ctrl->semEmpty))
	{
		if(errno == EINTR)
			return MXT_QUEUE_ERR_SYNC;
		else
			reportError("sem_wait", errno);
	}

	sys_pthread_mutex_lock(&ctrl->mtx);

	return MXT_QUEUE_ERR_OK;
}

int QR_post_poll_ST(QUEUE *this)
{
	--((Q_ST *) this->control)->count;

	return MXT_QUEUE_ERR_OK;
}

int QR_post_poll_SYNC(QUEUE *this)
{
	Q_SYNC *ctrl = (Q_SYNC *) this->control;
	
	sys_sem_post(&ctrl->semFill);

	sys_pthread_mutex_unlock(&ctrl->mtx);

	return MXT_QUEUE_ERR_OK;
}

QueueRoutine qrs_pre_poll[] = {
	&QR_pre_poll_ST,
	&QR_pre_poll_SYNC
};
QueueRoutine qrs_post_poll[] = {
	&QR_post_poll_ST,
	&QR_post_poll_SYNC
};

// # ---------------------------------------------------------------------------

int QR_delete_SYNC(QUEUE *this)
{
	Q_SYNC *ctrl = (Q_SYNC *) this->control;

	sys_sem_destroy(&ctrl->semFill);
	sys_sem_destroy(&ctrl->semEmpty);
	sys_pthread_mutex_destroy(&ctrl->mtx);

	free(ctrl);

	return MXT_QUEUE_ERR_OK;
}

QueueRoutine qrs_delete[] = {
	NULL,
	&QR_delete_SYNC
};

// # ---------------------------------------------------------------------------

int QR_size_ST(QUEUE *this)
{
	return ((Q_ST *) this->control)->count;
}

int QR_size_SYNC(QUEUE *this)
{
	int v;
	sys_sem_getvalue(&((Q_SYNC *) this->control)->semEmpty, &v);
	return v;
}

QueueRoutine qrs_size[] = {
	&QR_size_ST,
	&QR_size_SYNC
};

// # ===========================================================================

int QUEUE_new(QUEUE *this, size_t elemSize, size_t elemCount, int type)
{
	this->capacity = elemCount;
	this->begin = this->end = 0;
	this->size = elemSize;
	this->data = malloc(this->size * this->capacity);
	this->type = type;
	this->control = NULL;

	return call(qrs_new, this);
}

#define MXT_ADVANCE(i,s) (i=(i+1)%(s))
int QUEUE_push(QUEUE *this, const void *p)
{
	int r = call(qrs_pre_push, this);

	if(!r)
	{
		memcpy(this->data + this->end * this->size, p, this->size);
		MXT_ADVANCE(this->end, this->capacity);
		r = call(qrs_post_push, this);
	}

	return r;
}

int QUEUE_poll(QUEUE *this, void *p)
{
	int r = call(qrs_pre_poll, this);

	if(!r)
	{
		memcpy(p, this->data + this->begin * this->size, this->size);
		MXT_ADVANCE(this->begin, this->capacity);
		r = call(qrs_post_poll, this);
	}

	return r;
}
#undef MXT_ADVANCE

size_t QUEUE_size(QUEUE *this)
{
	return call(qrs_size, this);
}

void QUEUE_delete(QUEUE *this)
{
	call(qrs_delete, this);

	free(this->data);
}

