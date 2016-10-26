#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void) {
    int err;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    struct timespec tout;
    struct tm *tmp;
    char buf[64];

    pthread_mutex_lock(&lock);
    printf("mutex is locked\n");
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, 64, "%r", tmp);
    printf("current time is %s\n", buf);
    tout.tv_sec += 10;
    err = pthread_mutex_timedlock(&lock, &tout);
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, 64, "%r", tmp);
    printf("time now is %s\n", buf);
    if (err == 0) {
	printf("pthread mutex locked again!\n");
    } else {
	printf("can't lock mutex again:%s\n", strerror(err));
    }
    return 0;
}
