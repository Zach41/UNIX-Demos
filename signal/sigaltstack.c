#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void sigsegv_handler(int signo) {
    int x;
    printf("Caught signal %d (%s)\n", signo, strsignal(signo));
    printf("Top of handler stack near    %10p\n", (void *)&x);
    fflush(NULL);

    _exit(EXIT_FAILURE);	/* can't return after SIGSEGV */
}

static void overflowStack(int callNum) {
    char a[100000];

    printf("Call %4d - top of stack near %10p\n", callNum, &a[0]);

    overflowStack(callNum+1);
}

int main(void) {
    stack_t sigstack;
    struct sigaction sa;
    int j;

    printf("Top of standard stack near    %10p\n", (void *)&j);

    sigstack.ss_sp = malloc(SIGSTKSZ);
    if (sigstack.ss_sp == NULL) {
	perror("malloc error");
	exit(-1);
    }
    sigstack.ss_size = SIGSTKSZ;
    sigstack.ss_flags = 0;
    if (sigaltstack(&sigstack, NULL) == -1) {
	perror("sigaltstack error");
	exit(-1);
    }
    sa.sa_handler = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;	/* Handler uses alternative stack */
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
	perror("sigaction error");
	exit(-1);
    }

    overflowStack(1);

    return 0;
}
