#define _GNU_SOURCE
#include <signal.h>
#include <setjmp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "signal_functions.h"

static volatile sig_atomic_t canJump = 0;
static sigjmp_buf env;

static void sig_handler(int signo) {
    printf("Received signal: %d (%s), signal mask is:\n", signo, strsignal(signo));

    print_sigmask(stdout, NULL);

    if (!canJump) {
	printf("'env' buffer not set yet, doing a simple return\n");
	return;
    }

    siglongjmp(env, 1);
}

int main(void) {
    struct sigaction sa;

    print_sigmask(stdout, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
	perror("sigaction error");
	exit(-1);
    }

    if (sigsetjmp(env, 1) == 0) {
	canJump = 1;
    } else {
	print_sigmask(stdout, "After jump from handler, signal mask is:\n");
    }

    for (;;)
	pause();
    return 0;
}
