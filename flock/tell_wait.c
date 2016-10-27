#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "tell_wait.h"

static int pipefd1[2], pipefd2[2];

void _tell_wait() {
    if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0) {
	fprintf(stderr, "pipe error: %s\n", strerror(errno));
	exit(-1);
    }    
}

void _tell_parent() {
    if (write(pipefd1[1], "p", 1) != 1) {
	fprintf(stderr, "write error: %s\n", strerror(errno));
	exit(-1);
    }
}

void _wait_child() {
    char c;
    if (read(pipefd1[0], &c, 1) == 1) {
	if (c != 'p') {
	    fprintf(stderr, "wait parent error\n");
	    exit(-1);
	}
    } else {
	fprintf(stderr, "read error: %s\n", strerror(errno));
	exit (-1);
    }
}

void _tell_child() {
    if (write(pipefd2[1], "c", 1) != 1) {
	fprintf(stderr, "write error: %s\n", strerror(errno));
	exit(-1);
    }
}

void _wait_parent() {
    char c;
    if (read(pipefd2[0], &c, 1) == 1) {
	if (c != 'c') {
	    fprintf(stderr, "wait parent error\n");
	    exit(-1);
	}
    } else {
	fprintf(stderr, "read error: %s\n", strerror(errno));
	exit (-1);
    }
}
