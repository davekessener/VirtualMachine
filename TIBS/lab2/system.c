#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "system.h"

#define MXT_ERRBUFLEN 1024

#define MXT_GENERATE(n, e, a, b) void sys_ ## n a { e(n b, #n); }

void dft_error_routine(const char *, int);
void errno_error(int, const char *);
void direct_error(int, const char *);

error_routine_t error_routine = &dft_error_routine;

void setErrorRoutine(error_routine_t er)
{
	error_routine = er;
}

void installEmptySignalHandler(int s)
{
	installSignalHandler(s, installEmptySignalHandler);
}

void installSignalHandler(int s, sighandler_t h)
{
	struct sigaction sa;

	sa.sa_handler = h;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	sigaction(s, &sa, NULL);
}

void reportError(const char *s, int e)
{
	error_routine(s, e);
}

MXT_GENERATE(tcsetattr, errno_error, (int f, int a, struct termios *p), (f, a, p));
MXT_GENERATE(tcgetattr, errno_error, (int f, struct termios *p), (f, p));
MXT_GENERATE(sem_init, errno_error, (sem_t *s, int v), (s, 0, v));
MXT_GENERATE(sem_post, errno_error, (sem_t *s), (s));
MXT_GENERATE(sem_getvalue, errno_error, (sem_t *s, int *v), (s, v));
MXT_GENERATE(sem_destroy, errno_error, (sem_t *s), (s));
MXT_GENERATE(pthread_create, direct_error, (pthread_t *pf, void *(* fn)(void *), void *d), (pf, NULL, fn, d));
MXT_GENERATE(pthread_join, direct_error, (pthread_t pf), (pf, NULL));
MXT_GENERATE(pthread_kill, direct_error, (pthread_t pf, int s), (pf, s));
MXT_GENERATE(pthread_cond_init, direct_error, (pthread_cond_t *c), (c, NULL));
MXT_GENERATE(pthread_cond_wait, direct_error, (pthread_cond_t *c, pthread_mutex_t *m), (c, m));
MXT_GENERATE(pthread_cond_broadcast, direct_error, (pthread_cond_t *c), (c));
MXT_GENERATE(pthread_cond_destroy, direct_error, (pthread_cond_t *c), (c));
MXT_GENERATE(pthread_mutex_init, direct_error, (pthread_mutex_t *m), (m, NULL));
MXT_GENERATE(pthread_mutex_lock, direct_error, (pthread_mutex_t *m), (m));
MXT_GENERATE(pthread_mutex_unlock, direct_error, (pthread_mutex_t *m), (m));
MXT_GENERATE(pthread_mutex_destroy, direct_error, (pthread_mutex_t *m), (m));
MXT_GENERATE(clock_gettime, errno_error, (clockid_t id, struct timespec *ts), (id, ts));

// # ===========================================================================

void dft_error_routine(const char *s, int en)
{
	char errbuf[MXT_ERRBUFLEN];

	memset(errbuf, 0, MXT_ERRBUFLEN);

	snprintf(errbuf, MXT_ERRBUFLEN, "ERR: System function '%s' failed. ERRNO %i", s, en);

	errbuf[MXT_ERRBUFLEN - 1] = '\0';

	fprintf(stderr, "%s\n", errbuf);
	exit(EXIT_FAILURE);
}

void errno_error(int r, const char *f)
{
	if(r)
	{
		error_routine(f, errno);
	}
}

void direct_error(int e, const char *f)
{
	if(e)
	{
		error_routine(f, e);
	}
}

