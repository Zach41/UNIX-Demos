#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "pthread_functions.h"

void *fn(void *arg) {
    printf("thread run\n");

    return (void *)0;
}

int main(void) {
    int err;
    pthread_t tid;
    
    err = makethread(fn, NULL, &tid);
    if (err) {
	fprintf(stderr, "makethread error: %s\n", strerror(err));
	exit(-1);
    }

    // should fail
    err = pthread_join(tid, NULL);
    assert(err == EINVAL);

    return 0;
}
