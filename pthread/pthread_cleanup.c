#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void cleanup(void *arg) {
    char *msg = (char *)arg;

    printf("%s\n", msg);
}

void *thr_fn1(void *arg) {
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread1 first cleanup");
    pthread_cleanup_push(cleanup, "thread1 second cleanup");

    if (arg)
	return (void *)1;

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return (void *)1;
}

void *thr_fn2(void *arg) {
    printf("thread 2 start\n");
    
    pthread_cleanup_push(cleanup, "thread2 first cleanup");
    pthread_cleanup_push(cleanup, "thread2 second cleanup");

    if (arg)
	pthread_exit((void *)2);

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    
    pthread_exit((void *)2);
}

int main(void) {
    int err;
    pthread_t tid1, tid2;

    err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
    err = pthread_create(&tid2, NULL, thr_fn2, (void *)2);

    void *tret;
    err = pthread_join(tid1, &tret);
    printf("thread 1 ended with status: %ld\n", (long)tret);
    err = pthread_join(tid2, &tret);
    printf("thread2 ended with status: %ld\n", (long)tret);

    return 0;
}
