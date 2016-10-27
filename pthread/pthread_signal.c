#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int quitflag;
sigset_t mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void *thr_fn(void *arg) {
    int err, signo;

    for (;;) {
	err = sigwait(&mask, &signo);
	if (err != 0) {
	    fprintf(stderr, "sigwait failed: %s\n", strerror(err));
	    exit(-1);
	}

	switch (signo) {
	case SIGINT: {
	    printf("\ninterrupt\n");
	    break;
	}
	case SIGQUIT: {
	    pthread_mutex_lock(&lock);
	    quitflag = 1;
	    pthread_mutex_unlock(&lock);
	    pthread_cond_signal(&waitloc);

	    return (void *)0;
	}
	default:
	    printf("unexpected signal: %d\n", signo);
	    exit(-1);
	}
    }
}

int main(void) {
    int err;
    sigset_t oldmask;
    pthread_t tid;

    /* sigemptyset(&mask); */
    /* sigaddset(&mask, SIGQUIT); */
    /* sigaddset(&mask, SIGINT); */
    sigfillset(&mask);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) {
	fprintf(stderr, "SIG_BLOCK error: %s\n", strerror(err));
	exit(-1);
    }

    err = pthread_create(&tid, NULL, thr_fn, NULL);
    if (err != 0) {
	fprintf(stderr, "pthread_create error: %s\n", strerror(err));
	exit(-1);
    }

    pthread_mutex_lock(&lock);
    while (quitflag == 0) {
	pthread_cond_wait(&waitloc, &lock);
    }

    printf("SIGQUIT received\n");

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) != 0) {
	fprintf(stderr, "SIG_SETMASK error\n");
	exit(-1);
    }
    return 0;
}
