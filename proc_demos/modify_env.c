#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#define errExit(fmt, ...) do {fprintf(stderr, fmt, ##__VA_ARGS__); perror(""); exit(-1);} while (0)
extern char **environ;

int main(int argc, char **argv) {
    char **ep;

    clearenv();

    for (int i=1; i<argc; i++) {
	if (putenv(argv[i]) != 0)
	    errExit("putenv: %s", argv[i]);
    }

    if (setenv("GREET", "Hello world", 0) == -1)
	errExit("setenv");

    unsetenv("BYE");

    for (ep=environ; *ep!=NULL; ep++)
	puts(*ep);

    return 0;    
}
