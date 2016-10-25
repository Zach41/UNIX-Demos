#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static char *str2;
static int handled = 0;

static void sig_handler(int signo) {
    crypt(str2, "xx");
    handled++;
}

int main(int argc, char *argv[]) {
    char *cr1;
    int call_num, mismatched;
    struct sigaction sa;

    if (argc != 3) {
	fprintf(stderr, "Usage: %s str1 str2\n", argv[0]);
	exit(0);
    }

    str2 = argv[2];
    cr1 = strdup(crypt(argv[1], "xx"));

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_handler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
	perror("sigaction error");
	exit(-1);
    }

    for(call_num = 1, mismatched = 0; ; call_num++) {
	if(strcmp(crypt(argv[1], "xx"), cr1) != 0) {
	    mismatched++;
	    printf("Mismatch on call: %d (mismatch=%d handled=%d)\n", call_num, mismatched, handled);
	}
    }
    return 0;
}
