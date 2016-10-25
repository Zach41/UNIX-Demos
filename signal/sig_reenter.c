#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void sig_alarm(int);

static void sig_alarm(int signo) {
    struct passwd *rootptr;

    printf("In signal handler\n");
    if ((rootptr = getpwnam("root")) == NULL) {
	perror("getpwnam(root) error");
	exit(-1);
    }
    printf("root name: %s\n", rootptr -> pw_name);-0
    alarm(1);
}

int main(void) {
    struct passwd *ptr;
    signal(SIGALRM, sig_alarm);

    alarm(1);

    for (;;) {
	if ((ptr = getpwnam("Zach")) == NULL) {
	    perror("getpwnam(zach41) error");
	    exit(-1);
	}

	if (strcmp(ptr -> pw_name, "Zach") != 0) {
	    printf("return value corrupted!, pw_name = %s\n", ptr -> pw_name);
	}
    }

    return 0;
}
