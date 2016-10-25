#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "signal_functions.h"

int main(int argc, char *argv[])
{
    pid_t pid;
    int num_sigs, sig;
    if (argc < 4 || strcmp(argv[1], "--help") == 0) {
	printf("usage: %s pid num-sigs sig-num [sig-num-2]\n", argv[0]);
	exit(0);
    }
    sig = atoi(argv[3]);
    num_sigs = atoi(argv[2]);
    pid = atol(argv[1]);

    printf("%s: sending signal %d to process %ld %d times\n", argv[0], sig, (long)pid, num_sigs);

    for (int i=0; i<num_sigs; i++) {
	if (kill(pid, sig) == -1) {
	    perror("kill error");
	    exit(-1);
	}
    }

    if (argc > 4) {
	if (kill(pid, atoi(argv[4])) == -1) {
	    perror("kill error");
	    exit(-1);
	}
    }
    return 0;
}
