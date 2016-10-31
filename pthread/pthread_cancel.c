#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void cleanup(void *arg) {
    char *msg = (char *)arg;

    printf("%s\n", msg);
}

void *thr_fn1(void *arg) {
    printf("waiting for canceling.\n");

    /* pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); */
    
    pthread_cleanup_push(cleanup, "first cleanup");
    pthread_cleanup_push(cleanup, "second cleanup");
    /* must put this after cleanup push so cleanup can be executed */
    for (time_t t = time(NULL); t+5 > time(NULL); )
    	continue;
    pthread_testcancel();
    if (arg)
	return (void *)0;

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    return (void *)0;
}

void *thr_fn2(void *arg) {
    sleep(2);

    pthread_t tid2cancel = (pthread_t)arg;
    
    printf("cancel thread 1\n");
    int err = pthread_cancel(tid2cancel);
    if (err != 0) {
	fprintf(stderr, "pthread_cancel error: %s\n", strerror(err));
    }

    return (void *)0;
}

int main(void) {
    int err;
    pthread_t tid1, tid2;
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    err = pthread_create(&tid2, &attr, thr_fn2, (void *)tid1);

    pthread_attr_destroy(&attr);

    void *tret;
    err = pthread_join(tid1, &tret);
    if (tret == PTHREAD_CANCELED) {
	printf("thread 1 is canceled\n");
    } else {
	printf("some error occurred\n");
    }

    return 0;
}
