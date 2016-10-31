#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 2048

int main(void) {
    int n, int1, int2;
    char line[MAXLINE];

    while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0) {
	line[n] = 0;
	if (sscanf(line, "%d%d", &int1, &int2) == 2) {
	    sprintf(line, "%d\n", int1+int2);
	    n = strlen(line);
	    if (write(STDOUT_FILENO, line, n) != n) {
		perror("write error");
		exit(-1);
	    }
	} else {
	    if (write(STDOUT_FILENO, "invalid args\n", 13) != 13) {
		perror("write error");
		exit(-1);
	    }
	}
    }
    exit(0);
}
