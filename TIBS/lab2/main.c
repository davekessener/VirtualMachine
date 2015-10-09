#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include "queue.h"

#define FALSE 0
#define TRUE (!FALSE)

#define MXT_ELEMSIZE sizeof(char)
#define MXT_ELEMCOUNT 10
#define MXT_QUEUETYPE MXT_QUEUE_SYNC

#define MXT_TCTRL_UNLOCKED 0
#define MXT_TCTRL_LOCKED 1
#define MXT_TCTRL_QUIT 2

#define MXT_SLEEP_PRODUCER 3
#define MXT_SLEEP_CONSUMER 2

#define MXT_STR_PRODUCER1 "abcdefghijklmnopqrstuvwxyz"
#define MXT_STR_PRODUCER2 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define MXT_STR_RESETSTATUS "\r\e[6A"
#define MXT_STR_INACTIVE "\e[7;31;40mINACTIVE\e[0m"
#define MXT_STR_ACTIVE   "\e[1;32m ACTIVE \e[0m"

#define MXT_IDLEN 10
#define MXT_ID_P1 "Producer_1"
#define MXT_ID_P2 "Producer_2"
#define MXT_ID_CS "Consumer"
#define MXT_ID_CT "Control"

typedef struct
{
	pthread_mutex_t mtx;
	pthread_cond_t cond;
	int locked_status;
} TCTRL;

typedef struct
{
	TCTRL p1, p2, c;
	pthread_mutex_t mtx;
} STATUS;

typedef struct
{
	const char *s;
	size_t l;
} PRODUCER_t;

typedef struct
{
	QUEUE *buf;
	TCTRL *sem;
	const int *should_quit;
	void *data;
} THREAD_DATA;

void *doProducerThread(void *);
void *doConsumerThread(void *);
void *doControlThread(void *);

void initializeStatus(void);
void printStatus(void);
void printCharacter(char);

void creation_error(const char *, int);

void TCTRL_check(TCTRL *);
void TCTRL_toggle(TCTRL *);
void TCTRL_disable(TCTRL *);
void TCTRL_sleep(TCTRL *, int);

STATUS thread_status;
pthread_mutex_t print_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_t producer1Thread, producer2Thread, consumerThread, controlThread;

void handle_sigusr1(int s)
{
	struct sigaction sa;

	sa.sa_handler = handle_sigusr1;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGUSR1, &sa, NULL);
}

int main(int argc, char *argv[])
{
	QUEUE disp_buf;
	sem_t running;
	int quit_now;
	THREAD_DATA p1 = { .buf = &disp_buf, .sem = &thread_status.p1, .should_quit = &quit_now},
				p2 = { .buf = &disp_buf, .sem = &thread_status.p2, .should_quit = &quit_now},
				cs = { .buf = &disp_buf, .sem = &thread_status.c,  .should_quit = &quit_now},
				ct = { .buf = NULL,      .sem = NULL,              .should_quit = &quit_now};
	int v;

	PRODUCER_t p1_data = {.s = MXT_STR_PRODUCER1, .l = sizeof(MXT_STR_PRODUCER1) - 1};
	PRODUCER_t p2_data = {.s = MXT_STR_PRODUCER2, .l = sizeof(MXT_STR_PRODUCER2) - 1};

	p1.data = &p1_data;
	p2.data = &p2_data;
	ct.data = &running;

	handle_sigusr1(SIGUSR1);

	QUEUE_new(&disp_buf, MXT_ELEMSIZE, MXT_ELEMCOUNT, MXT_QUEUETYPE);
	initializeStatus();
	initializeTerminal();
	sem_init(&running, 0, 0);
	quit_now = 0;

	if(v = pthread_create(&producer1Thread, NULL, &doProducerThread, &p1)) creation_error(MXT_ID_P1, v);
	if(v = pthread_create(&producer2Thread, NULL, &doProducerThread, &p2)) creation_error(MXT_ID_P2, v);
	if(v = pthread_create(&consumerThread,  NULL, &doConsumerThread, &cs)) creation_error(MXT_ID_CS, v);
	if(v = pthread_create(&controlThread,   NULL, &doControlThread,  &ct)) creation_error(MXT_ID_CT, v);

	sem_wait(&running);

	quit_now = 1;

	TCTRL_disable(&thread_status.p1);
	TCTRL_disable(&thread_status.p2);
	TCTRL_disable(&thread_status.c);

	pthread_kill(producer1Thread, SIGUSR1);
	pthread_kill(producer2Thread, SIGUSR1);
	pthread_kill(consumerThread, SIGUSR1);

	pthread_join(producer1Thread, NULL);
	pthread_join(producer2Thread, NULL);
	pthread_join(consumerThread, NULL);
	pthread_join(controlThread, NULL);

	QUEUE_delete(&disp_buf);

	restoreTerminal();

	printf("\nGoodbye.\n");

	return EXIT_SUCCESS;
}

void *doProducerThread(void *p)
{
	THREAD_DATA *d = p;
	PRODUCER_t *ptrn = d->data;
	int i = 0;

	while(TRUE)
	{
		TCTRL_check(d->sem);

		if(*d->should_quit) break;

		if(!QUEUE_push(d->buf, &ptrn->s[i]))
		{
			i = (i + 1) % ptrn->l;
			TCTRL_sleep(d->sem, MXT_SLEEP_PRODUCER);
		}
	}
}

void *doConsumerThread(void *p)
{
	THREAD_DATA *d = p;

	while(TRUE)
	{
		char c;
		
		TCTRL_check(d->sem);

		if(*d->should_quit) break;

		if(!QUEUE_poll(d->buf, &c))
		{
			printCharacter(c);
			TCTRL_sleep(d->sem, MXT_SLEEP_CONSUMER);
		}
	}
}

void *doControlThread(void *p)
{
	THREAD_DATA *d = p;

	while(!*d->should_quit)
	{
		char c;

		if(readTerminal(&c) < 0)
		{
			printf("\nERROR!\n");
		}
		else switch(c)
		{
			case 'q':
			case 'Q':
				sem_post((sem_t *) d->data);
				pthread_exit(NULL);
				break;
			case '1':
				TCTRL_toggle(&thread_status.p1);
				pthread_kill(producer1Thread, SIGUSR1);
				break;
			case '2':
				TCTRL_toggle(&thread_status.p2);
				pthread_kill(producer2Thread, SIGUSR1);
				break;
			case 'c':
			case 'C':
				TCTRL_toggle(&thread_status.c);
				pthread_kill(consumerThread, SIGUSR1);
				break;
		}

		printStatus();
	}
}

void TCTRL_check(TCTRL *s)
{
	pthread_mutex_lock(&s->mtx);
	while(s->locked_status == MXT_TCTRL_LOCKED)
	{
		pthread_cond_wait(&s->cond, &s->mtx);
	}
	pthread_mutex_unlock(&s->mtx);
}

void TCTRL_toggle(TCTRL *s)
{
	pthread_mutex_lock(&s->mtx);
	s->locked_status = !s->locked_status;
	if(s->locked_status == MXT_TCTRL_UNLOCKED)
	{
		pthread_cond_signal(&s->cond);
	}
	pthread_mutex_unlock(&s->mtx);
}

void TCTRL_disable(TCTRL *s)
{
	pthread_mutex_lock(&s->mtx);
	s->locked_status = MXT_TCTRL_QUIT;
	pthread_cond_broadcast(&s->cond);
	pthread_mutex_unlock(&s->mtx);
}

void TCTRL_sleep(TCTRL *s, int t)
{
	pthread_mutex_lock(&s->mtx);
	if(s->locked_status == MXT_TCTRL_UNLOCKED)
	{
		struct timespec ts;
		int r;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += t;

		while(s->locked_status == MXT_TCTRL_UNLOCKED)
		{
			if((r = pthread_cond_timedwait(&s->cond, &s->mtx, &ts)) == ETIMEDOUT ||
				r == EINVAL)
				break;
		}
	}
	pthread_mutex_unlock(&s->mtx);
}

void initializeStatus(void)
{
	pthread_mutex_init(&thread_status.mtx, NULL);
	pthread_cond_init(&thread_status.p1.cond, NULL);
	pthread_mutex_init(&thread_status.p1.mtx, NULL);
	thread_status.p1.locked_status = MXT_TCTRL_UNLOCKED;
	pthread_cond_init(&thread_status.p2.cond, NULL);
	pthread_mutex_init(&thread_status.p2.mtx, NULL);
	thread_status.p1.locked_status = MXT_TCTRL_UNLOCKED;
	pthread_cond_init(&thread_status.c.cond, NULL);
	pthread_mutex_init(&thread_status.c.mtx, NULL);
	thread_status.p1.locked_status = MXT_TCTRL_UNLOCKED;

	printf("\n\n\n\n\n\n");

	printStatus();
}

void printStatus(void)
{
	int producer1Active = !thread_status.p1.locked_status,
		producer2Active = !thread_status.p2.locked_status,
		consumerActive  = !thread_status.c.locked_status;

	pthread_mutex_lock(&print_mtx);
	printf("%sStatus:\n"
		   "%*s [%s]  (Press '1' to toggle)\n"
		   "%*s [%s]  (Press '2' to toggle)\n"
		   "%*s [%s]  (Press 'C' to toggle)\n"
		   "Press 'Q' to quit.\n\n",
		   MXT_STR_RESETSTATUS,
		   MXT_IDLEN, MXT_ID_P1, producer1Active ? MXT_STR_ACTIVE : MXT_STR_INACTIVE,
		   MXT_IDLEN, MXT_ID_P2, producer2Active ? MXT_STR_ACTIVE : MXT_STR_INACTIVE,
		   MXT_IDLEN, MXT_ID_CS, consumerActive ? MXT_STR_ACTIVE : MXT_STR_INACTIVE);
	pthread_mutex_unlock(&print_mtx);
}

void printCharacter(char c)
{
	pthread_mutex_lock(&print_mtx);
	printf("\rLast character removed: %c", c);
	fflush(stdout);
	pthread_mutex_unlock(&print_mtx);
}

void creation_error(const char *id, int e)
{
	pthread_mutex_lock(&print_mtx);

	fprintf(stderr, "ERR: Couldn't start thread '%s'; ERRNO %i\n", id, e);

	exit(EXIT_FAILURE);
}

