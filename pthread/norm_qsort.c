#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUMNUM 8000000L

long nums[NUMNUM];

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

int main(void) {
    struct timeval start, end;
    long long startusec, endusec;
    
    srandom(1);
    for (long long i=0; i<NUMNUM; i++) {
	nums[i] = random();
    }

    gettimeofday(&start, NULL);
    
    qsort(nums, NUMNUM, sizeof(long), cmplong);
    
    gettimeofday(&end, NULL);

    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec   = end.tv_sec * 1000000 + end.tv_usec;

    double elapsed = (endusec - startusec) / 1000000.0;
    printf("normal sort time: %.4f\n", elapsed);
    
    return 0;
}

