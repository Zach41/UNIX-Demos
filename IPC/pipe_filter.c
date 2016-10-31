#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 2048

int main(void) {
    char line[MAXLINE];
    FILE *fpin;

    if ((fpin = popen("./myuclc", "r")) == NULL) {
	perror("fopen error");
	exit(-1);
    }

    for (;;) {
	fputs("prompt> ", stdout);
	fflush(stdout);
	if (fgets(line, MAXLINE, fpin) == NULL) {
	    break;
	}
	if (fputs(line, stdout) == EOF) {
	    perror("fputs error");
	    exit(-1);
	}
    }
    if (ferror(fpin)) {
	perror("fgets error");
	exit(-1);
    }

    putchar('\n');
    exit(0);
}
