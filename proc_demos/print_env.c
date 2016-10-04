#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(void) {
    char **ep;

    for (ep = environ; *ep != NULL; ep++)
	puts(*ep);
    exit(0);
}
