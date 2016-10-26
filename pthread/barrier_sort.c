#include <stdio.h>
#include <limits.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NTHR   8
#define NUMNUM 8000000L
#define TNUM   (NUMNUM / NTHR)

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t barrier;

int cmplong(const void *arg1, const void *arg2) {
    long l1 = *(long *)arg1;
    long l2 = *(long *)arg2;

    if (l1 == l2)
	return 0;
    else if (l1 < l2)
	return -1;
    else
	return 1;
}

void *thr_fn(void *arg) {
    long idx = (long)arg;

    qsort(&nums[idx], TNUM, sizeof(long), cmplong);
    pthread_barrier_wait(&barrier);

    return (void *)0;
}

void merge() {
    long idx[NTHR];
    long minidx, num;

    for (int i=0; i<NTHR; i++)
	idx[i] = i*TNUM;
    for (int sidx = 0; sidx<NUMNUM; sidx++) {
	num = LONG_MAX;
	for (int i=0; i<NTHR; i++) {
	    if (idx[i] < (i+1)*TNUM && num > nums[idx[i]]) {
		num = nums[idx[i]];
		minidx = i;
	    }
	}

	snums[sidx] = num;
	idx[minidx] += 1;
    }
}

int main(void) {
    int err;
    pthread_t tid;
    struct timeval start, end;
    long long startusec, endusec;
    double elapsed;

    srandom(1);
    for (long long i=0; i<NUMNUM; i++) {
	nums[i] = random();
    }

    gettimeofday(&start, NULL);
    pthread_barrier_init(&barrier, NULL, NTHR+1);
    for (int i=0; i<NTHR; i++) {
	err = pthread_create(&tid, NULL, thr_fn, (void *)(i*TNUM));
	if (err != 0) {
	    fprintf(stderr, "pthread_create error: %s\n", strerror(err));
	    exit(-1);
	}
    }
    pthread_barrier_wait(&barrier);
    merge();
    gettimeofday(&end, NULL);

    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec   = end.tv_sec * 1000000 + end.tv_usec;
    elapsed   = (endusec - startusec) / 1000000.0;
    printf("sort took %.4f seconds\n", elapsed);
    for (int i=0; i<NUMNUM; i++) {
	fprintf(stderr, "%ld\n", snums[i]);
    }
    return 0;
}
