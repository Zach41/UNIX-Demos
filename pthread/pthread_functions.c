#include <pthread.h>

int makethread(void *(*fn)(void *), void *arg, pthread_t *tidp) {
    int err;
    pthread_t tid;
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    if (err != 0)
	return err;
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0) {
	pthread_create(&tid, &attr, fn, arg);
	*tidp = tid;
    }
    pthread_attr_destroy(&attr);
    
    return err;
}
