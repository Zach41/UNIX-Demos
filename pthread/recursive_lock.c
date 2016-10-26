#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "pthread_functions.h"

struct to_info {
    void            *(*fn)(void *);
    void            *to_arg;
    int             to_wait;
};

void *timer_helper(void *arg) {
    struct to_info *tip;

    tip = (struct to_info *)arg;
    printf("sleep for %d secs\n", tip -> to_wait);
    sleep(tip -> to_wait);
    (tip -> fn)(tip -> to_arg);
    free(arg);

    return (void *)0;
}

void timeout(const int when, void *(*fn)(void *), void *arg) {
    struct to_info *tip;

    tip = (struct to_info *)malloc(sizeof(struct to_info));
    if (tip != NULL) {
	tip -> to_wait = when;
	tip -> fn = fn;
	tip -> to_arg = arg;

	pthread_t tid;
	int err = makethread(timer_helper, (void *)tip, &tid);

	if (err != 0) {
	    fprintf(stderr, "makethread error: %s\n", strerror(err));
	    free(tip);
	    exit(-1);	    
	}
    }
}

pthread_mutex_t lock;
pthread_mutexattr_t attr;

static volatile int flag = 0;

void *retry(void *arg) {
    pthread_mutex_lock(&lock);

    printf("Recursive lock\n");
    flag = 1;
    pthread_mutex_unlock(&lock);
    
    return (void *)0;
}

int main(void) {
    int err;

    err = pthread_mutexattr_init(&attr);
    if (err != 0) {
	fprintf(stderr, "pthread_mutexattr_init error: %s\n", strerror(err));
	exit(-1);
    }

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    if (err != 0) {
	fprintf(stderr, "pthread_mutexattr_settype error: %s\n", strerror(err));
	exit(-1);
    }

    err = pthread_mutex_init(&lock, &attr);
    if (err != 0) {
	fprintf(stderr, "pthread_mutex_init error: %s\n", strerror(err));
	exit(-1);
    }
    
    pthread_mutex_lock(&lock);

    timeout(4, retry, NULL);

    pthread_mutex_unlock(&lock);

    while (!flag)
	;

    return 0;
}
