#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "signal_functions.h"

int main(int argc, char *argv[])
{
    int sig;
    siginfo_t si;
    sigset_t allSigs;

    printf("%s: PID is %ld\n", argv[0], (long)getpid());

    sigfillset(&allSigs);
    if (sigprocmask(SIG_SETMASK, &allSigs, NULL) == -1) {
	perror("sigprocmask error");
	exit(-1);
    }
    printf("%s: all signal blocked\n", argv[0]);

    if (argc > 1) {
	printf("%s: about to delay %s seconds\n", argv[0], argv[1]);
	sleep(atoi(argv[1]));
	printf("%s: finished delay\n", argv[0]);
    }

    for (;;) {
	sig = sigwaitinfo(&allSigs, &si);
	if (sig == -1) {
	    perror("sigwaitinfo error");
	    exit(-1);
	}

	if (sig == SIGINT || sig == SIGTERM) {
	    printf("terminating\n");
	    exit(0);
	}

	printf("got signal %d (%s)\n", sig, strsignal(sig));
	printf("\t\tsi_signo = %d, si_code = %d (%s), si_value = %d, ",
	       si.si_signo, si.si_code,
	       si.si_code == SI_USER ? "SI_USER" : (si.si_code == SI_QUEUE ? "SI_QUEUE" : "other"),
	       si.si_value.sival_int);
	printf("si_pid = %ld, si_uid = %ld\n", (long)si.si_pid, (long)si.si_uid);
    }
    return 0;
}
