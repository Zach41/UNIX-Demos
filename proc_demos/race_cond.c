#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int pipefd[2];

static void tell_wait();
static void tell_parent(pid_t);
static void tell_child(pid_t);
static void wait_child();
static void wati_parent();

static void charatatime(const char*);

int main(void) {
    pid_t pid;
    
    tell_wait();

    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid == 0) {
	charatatime("output from child\n");
	tell_parent(getppid());
    } else {
	wait_child();
	charatatime("output from parent\n");
    }
    exit(0);
}

static void charatatime(const char* ptr) {
    setbuf(stdout, NULL);
    for (; *ptr != 0; ptr++) {
	sleep(1);
	putc(*ptr, stdout);
    }
}

static void tell_wait() {
    if (pipe(pipefd) < 0) {
	perror("pipe error");
	exit(-1);
    }
}

static void tell_parent(pid_t pid) {
    if (write(pipefd[1], "p", 1) != 1) {
	perror("write error");
	exit(-1);
    }
}

static void wait_child() {
    char c;
    int n;
    if ((n = read(pipefd[0], &c, 1)) < 0) {
	perror("read error");
	exit(-1);
    }
    if (c != 'p') {
	fprintf(stderr, "wait child error");
	exit(-1);
    }
}
