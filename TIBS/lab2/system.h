#ifndef DAV_BS_LAB2_SYSTEM
#define DAV_BS_LAB2_SYSTEM

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

typedef void (* error_routine_t)(const char *, int);
typedef void (* sighandler_t)(int);

void setErrorRoutine(error_routine_t);
void installEmptySignalHandler(int);
void installSignalHandler(int, sighandler_t);
void reportError(const char *, int);

void sys_tcsetattr(int, int, struct termios *);
void sys_tcgetattr(int, struct termios *);

void sys_sem_init(sem_t *, int);
void sys_sem_post(sem_t *);
void sys_sem_getvalue(sem_t *, int *);
void sys_sem_destroy(sem_t *);

void sys_pthread_create(pthread_t *, void *(*)(void *), void *);
void sys_pthread_join(pthread_t);
void sys_pthread_kill(pthread_t, int);

void sys_pthread_cond_init(pthread_cond_t *);
void sys_pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
void sys_pthread_cond_broadcast(pthread_cond_t *);
void sys_pthread_cond_destroy(pthread_cond_t *);

void sys_pthread_mutex_init(pthread_mutex_t *);
void sys_pthread_mutex_lock(pthread_mutex_t *);
void sys_pthread_mutex_unlock(pthread_mutex_t *);
void sys_pthread_mutex_destroy(pthread_mutex_t *);

void sys_clock_gettime(clockid_t, struct timespec *);

#endif

