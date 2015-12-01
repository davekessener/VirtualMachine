#include <signal.h>
#include "system.h"
#include "manager.h"
#include "thread_control.h"
#include "print.h"
#include "terminal.h"
#include "queue.h"

#define FALSE 0
#define TRUE (!FALSE)

#define MXT_ELEMSIZE sizeof(char)
#define MXT_ELEMCOUNT 10
#define MXT_QUEUETYPE MXT_QUEUE_SYNC

#define MXT_STR_PRODUCER1 "abcdefghijklmnopqrstuvwxyz"
#define MXT_STR_PRODUCER2 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

#define MXT_SLEEP_PRODUCER 3000
#define MXT_SLEEP_CONSUMER 2000

typedef struct
{
	QUEUE *queue;
	const char *s;
	size_t l;
} producer_data_t;

typedef struct
{
	tid_t p1, p2, c;
} control_data_t;

void doProducerThread(TMANAGER *, tid_t, TCTRL *, void *);
void doConsumerThread(TMANAGER *, tid_t, TCTRL *, void *);
void doControlThread(TMANAGER *, tid_t, TCTRL *, void *);

void updateStatus(TMANAGER *, tid_t, tid_t, tid_t);

void initialize(void);
void cleanup(void);

int main(int argc, char *argv[])
{
	QUEUE disp_buf;
	TMANAGER manager;
	producer_data_t p1, p2;
	control_data_t ctrl;

	QUEUE_new(&disp_buf, MXT_ELEMSIZE, MXT_ELEMCOUNT, MXT_QUEUETYPE);
	TMANAGER_new(&manager);

	p1.queue = &disp_buf;
	p1.s = MXT_STR_PRODUCER1;
	p1.l = sizeof(MXT_STR_PRODUCER1) - 1;

	p2.queue = &disp_buf;
	p2.s = MXT_STR_PRODUCER2;
	p2.l = sizeof(MXT_STR_PRODUCER2) - 1;

	ctrl.p1 = TMANAGER_run(&manager, doProducerThread, &p1);
	ctrl.p2 = TMANAGER_run(&manager, doProducerThread, &p2);
	ctrl.c  = TMANAGER_run(&manager, doConsumerThread, &disp_buf);
	TMANAGER_run(&manager, doControlThread, &ctrl);

	initialize();
	TMANAGER_wait(&manager);
	cleanup();

	TMANAGER_delete(&manager);
	QUEUE_delete(&disp_buf);

	printf("\nGoodbye.\n");

	return EXIT_SUCCESS;
}

void initialize(void)
{
	installEmptySignalHandler(SIGUSR1);
	initializeTerminal();
	printf("\n\n\n\n\n\n\n");
	printStatus(MXT_TMANAGER_RUNNING, MXT_TMANAGER_RUNNING, MXT_TMANAGER_RUNNING);
}

void cleanup(void)
{
	restoreTerminal();
}

void doProducerThread(TMANAGER *m, tid_t id, TCTRL *ctrl, void *p)
{
	producer_data_t *data = p;
	int i = 0;

	while(TRUE)
	{
		TCTRL_check(ctrl);

		if(!TMANAGER_running(m)) break;

		if(!QUEUE_push(data->queue, &data->s[i]))
		{
			i = (i + 1) % data->l;
			printQueue(data->queue);
			TCTRL_sleep(ctrl, MXT_SLEEP_PRODUCER);
		}
	}
}

void doConsumerThread(TMANAGER *m, tid_t id, TCTRL *ctrl, void *p)
{
	QUEUE *queue = p;

	while(TRUE)
	{
		char c;
		
		TCTRL_check(ctrl);

		if(!TMANAGER_running(m)) break;

		if(!QUEUE_poll(queue, &c))
		{
			printQueue(queue);
			printCharacter(c);
			TCTRL_sleep(ctrl, MXT_SLEEP_CONSUMER);
		}
	}
}

void doControlThread(TMANAGER *m, tid_t id, TCTRL *ctrl, void *p)
{
	control_data_t *status = p;

	while(TMANAGER_running(m))
	{
		TCTRL_check(ctrl);

		switch(readTerminal())
		{
			case 'q':
			case 'Q':
				TMANAGER_shutdown(m);
				return;
			case '1':
				TMANAGER_toggle(m, status->p1);
				break;
			case '2':
				TMANAGER_toggle(m, status->p2);
				break;
			case 'c':
			case 'C':
				TMANAGER_toggle(m, status->c);
				break;
		}

		updateStatus(m, status->p1, status->p2, status->c);
	}
}

void updateStatus(TMANAGER *m, tid_t p1, tid_t p2, tid_t c)
{
	printStatus(TMANAGER_status(m, p1), TMANAGER_status(m, p2), TMANAGER_status(m, c));
}

