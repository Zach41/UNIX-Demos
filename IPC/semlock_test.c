#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "sem_lock.h"

static int cnt;
struct slock *s_mutex;

void *thr_fn1(void *arg) {
    for (int i=0; i<1000000; i++) {
	s_lock(s_mutex);
	cnt += 1;
	s_unlock(s_mutex);
    }

    return (void *)0;
}

void *thr_fn2(void *arg) {
    for (int i=0; i<1000000; i++) {
	s_lock(s_mutex);
	cnt += 1;
	s_unlock(s_mutex);
    }

    return (void *)0;
}

int main(void) {
    int err;
    pthread_t tid1, tid2;
    
    s_mutex = s_alloc();
    if (s_mutex == NULL) {
	perror("slock allocation failed");
	exit(-1);
    }

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);

    err = pthread_join(tid1, NULL);
    err = pthread_join(tid2, NULL);

    printf("CNT: %d\n", cnt);

    s_free(s_mutex);

    return 0;
}
