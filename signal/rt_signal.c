#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sig, numSigs, sigData;
    union sigval sv;

    if (argc < 4 || strcmp(argv[1], "--help") == 0) {
	printf("Usage: %s pid sig-num data [num-sigs]\n", argv[0]);
	exit(0);
    }

    printf("%s: PID is %ld, UID is %ld\n", argv[0], (long)getpid(), (long)getuid());

    sig = atoi(argv[2]);
    sigData = atoi(argv[3]);
    numSigs = (argc > 4) ? atoi(argv[4]) : 1;

    for (int i=0; i<numSigs; i++) {
	sv.sival_int = sigData + i;
	if (sigqueue(atol(argv[1]), sig, sv) == -1) {
	    perror("sigqueue error");
	    exit(-1);
	}
    }
    
    return 0;
}
