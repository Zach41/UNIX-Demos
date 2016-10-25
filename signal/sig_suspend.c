#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "signal_functions.h"

static volatile sig_atomic_t gotSigquit = 0;

static void handler(int signo) {
    printf("Caught signal %d (%s)\n", signo, strsignal(signo));

    if (signo == SIGQUIT)
	gotSigquit = 1;
}

int main(int argc, char *argv[])
{
    int loopNum;
    time_t start_time;
    sigset_t origMask, blockMask;
    struct sigaction sa;

    print_sigmask(stdout, "Initial signal mask is:\n");
    
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGQUIT);
    sigaddset(&blockMask, SIGINT);
    if (sigprocmask(SIG_SETMASK, &blockMask, &origMask) == -1) {
	perror("sigprocmask error");
	exit(-1);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
	perror("sigaction error");
	exit(-1);
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
	perror("sigaction error");
	exit(-1);
    }

    for (loopNum = 1; !gotSigquit; loopNum++) {
	printf("=== LOOP %d\n", loopNum);
	
	print_sigmask(stdout, "Starting critical section, signal mask is:\n");

	for (start_time = time(NULL); time(NULL) < start_time + 4; ) {
	    continue;
	}

	print_pendingsigs(stdout, "Before sigsuspend() - pending signals:\n");

	if (sigsuspend(&origMask) == -1 && errno != EINTR) {
	    perror("sigsuspend error");
	    exit(-1);
	}
    }
    
    return 0;
}
