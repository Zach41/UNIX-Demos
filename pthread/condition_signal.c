#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct task {
    char msg[64];
};

struct task *tp;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock  = PTHREAD_MUTEX_INITIALIZER;

void delay(int secs) {
    time_t t;
    for (t=time(NULL); t+secs>time(NULL); )
	;
}

void *process_task(void *arg) {
    pthread_mutex_lock(&qlock);
    while (tp == NULL)
	pthread_cond_wait(&qready, &qlock);
    // ready to go
    printf("Task message: %s\n", tp -> msg);
    pthread_mutex_unlock(&qlock);

    return (void *)0;
}

void *enqueue_msg(void *arg) {
    char *msg = "Hello, there";
    pthread_mutex_lock(&qlock);
    delay(5);
    tp = (struct task *)malloc(sizeof(struct task));
    memcpy(tp -> msg, msg, strlen(msg)+1);
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);

    return (void *)0;
}

int main(void) {
    int err;
    pthread_t tid1, tid2;

    err = pthread_create(&tid1, NULL, process_task, NULL);
    if (err != 0) {
	fprintf(stderr, "pthread_create error: %s\n", strerror(err));
	exit(-1);
    }
    err = pthread_create(&tid2, NULL, enqueue_msg, NULL);
    if (err != 0) {
	fprintf(stderr, "pthread_create error: %s\n", strerror(err));
	exit(-1);
    }

    void *tret;
    err = pthread_join(tid1, &tret);
    if (err != 0) {
	fprintf(stderr, "pthread_join error: %s\n", strerror(err));
	exit(-1);
    }
    printf("thread1 exited with status: %ld\n", (long)tret);

    err = pthread_join(tid2, &tret);
    if (err != 0) {
	fprintf(stderr, "pthread_join error: %s\n", strerror(err));
	exit(-1);
    }
    printf("thread1 exited with status: %ld\n", (long)tret);

    return 0;
}

