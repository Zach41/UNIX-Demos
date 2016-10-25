#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "signal_functions.h"

static int sig_cnt[NSIG];
static volatile sig_atomic_t got_sigint = 0;

static void sig_handler(int signo) {
    if (signo == SIGINT)
	got_sigint = 1;
    else
	sig_cnt[signo]++;
}

int main(int argc, char *argv[])
{
    int num_secs;
    sigset_t pending_mask, blocking_mask, empty_mask;
    
    printf("%ss: PID is %ld\n", argv[0], (long)getpid());

    for (int n=1; n<NSIG; n++)
	signal(n, sig_handler);

    if (argc > 1) {
	num_secs = atoi(argv[1]);

	sigfillset(&blocking_mask);
	if (sigprocmask(SIG_SETMASK, &blocking_mask, NULL) == -1) {
	    perror("sigprocmask error");
	    exit(-1);
	}

	printf("%s: sleeping for %d seconds\n", argv[0], num_secs);
	sleep(num_secs);

	if (sigpending(&pending_mask) == -1) {
	    perror("sigpending error");
	    exit(-1);
	}

	printf("%s: pending signals are: \n", argv[0]);
	print_sigset(stdout, "\t\t", &pending_mask);

	sigemptyset(&empty_mask);
	if (sigprocmask(SIG_SETMASK, &empty_mask, NULL) == -1) {
	    perror("sigprocmask error");
	    exit(-1);
	}
    }

    while (!got_sigint)
	;
    
    for (int n=1; n<NSIG; n++) {
	if (sig_cnt[n] != 0)
	    printf("%s: signal %d caught %d time%s\n", argv[0], n, sig_cnt[n], sig_cnt[n] == 1? "" : "s");
    }
    return 0;
}
