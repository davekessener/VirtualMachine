#include <pthread.h>
#include "print.h"
#include "system.h"

#define MXT_STR_RESETSTATUS "\r\e[7A"
#define MXT_STR_INACTIVE "\e[7;31;40mINACTIVE\e[0m"
#define MXT_STR_ACTIVE   "\e[1;32m ACTIVE \e[0m"

#define MXT_IDLEN 10
#define MXT_ID_P1 "Producer_1"
#define MXT_ID_P2 "Producer_2"
#define MXT_ID_CS "Consumer"
#define MXT_ID_CT "Control"

typedef struct
{
	sem_t semFill, semEmpty;
	pthread_mutex_t mtx;
} Q_SYNC;

pthread_mutex_t print_mtx = PTHREAD_MUTEX_INITIALIZER;

void printStatus(int p1A, int p2A, int cA)
{
	sys_pthread_mutex_lock(&print_mtx);
	printf("%sStatus:\n"
		   "%*s [%s]  (Press '1' to toggle)\n"
		   "%*s [%s]  (Press '2' to toggle)\n"
		   "%*s [%s]  (Press 'C' to toggle)\n"
		   "Press 'Q' to quit.\n\n\n",
		   MXT_STR_RESETSTATUS,
		   MXT_IDLEN, MXT_ID_P1, p1A ? MXT_STR_ACTIVE : MXT_STR_INACTIVE,
		   MXT_IDLEN, MXT_ID_P2, p2A ? MXT_STR_ACTIVE : MXT_STR_INACTIVE,
		   MXT_IDLEN, MXT_ID_CS, cA ? MXT_STR_ACTIVE : MXT_STR_INACTIVE);
	sys_pthread_mutex_unlock(&print_mtx);
}

void printQueue(QUEUE *q)
{
	sys_pthread_mutex_lock(&print_mtx);
	sys_pthread_mutex_lock(&((Q_SYNC *) q->control)->mtx);
	
	printf("\e[1AQueue contains [");
	{
		const char *s = q->data;
		int i, l;

		sys_sem_getvalue(&((Q_SYNC *) q->control)->semEmpty, &l);

		for(i = 0 ; i < l ; ++i)
		{
			printf("%c", s[(q->begin + i) % q->capacity]);
		}
		for(i = 0 ; i < q->capacity - l ; ++i)
		{
			printf(" ");
		}
		printf("]");
	}
	printf("\n");
	fflush(stdout);

	sys_pthread_mutex_unlock(&((Q_SYNC *) q->control)->mtx);
	sys_pthread_mutex_unlock(&print_mtx);
}

void printCharacter(char c)
{
	sys_pthread_mutex_lock(&print_mtx);
	printf("Last character removed: %c\r", c);
	fflush(stdout);
	sys_pthread_mutex_unlock(&print_mtx);
}

