#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "pthread_functions.h"

void *thr_fn1(void *arg) {
    char *value1 = getenv_("NAME");
    printf("NAME: %s (%p)\n", value1, value1);
    char *value2 = getenv_("ID");
    printf("ID: %s (%p)\n", value2, value2);

    sleep(2);

    return (void *)0;
}

void *thr_fn2(void *arg) {
    char *value1 = getenv_("NAME");
    printf("NAME: %s (%p)\n", value1, value1);
    char *value2 = getenv_("ID");
    printf("ID: %s (%p)\n", value2, value2);

    sleep(2);
    
    return (void *)0;
}

int main(int argc, char *argv[])
{
    int err;
    pthread_attr_t attr;
    pthread_t tid1, tid2;
    
    /* err = pthread_attr_init(&attr); */
    /* err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); */

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);

    err = pthread_join(tid1, NULL);
    err = pthread_join(tid2, NULL);
    
    return 0;
}

