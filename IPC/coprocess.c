#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define MAXLINE 2048

static void sig_pipe(int signo);

int main(void) {
    int n, fd1[2], fd2[2];
    pid_t pid;
    char line[MAXLINE];
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGPIPE);
    sa.sa_flags = 0;
    sa.sa_handler = sig_pipe;
    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
	perror("sigaction error");
	exit(-1);
    }

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
	perror("pipe error");
	exit(-1);
    }

    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid > 0) {
	// 父进程
	close(fd1[0]);
	close(fd2[1]);

	while (fgets(line, MAXLINE, stdin) != NULL) {
	    n = strlen(line);
	    if (write(fd1[1], line, n) != n) {
		perror("write to pipe error");
		exit(-1);
	    }
	    if ((n = read(fd2[0], line, MAXLINE)) < 0) {
		perror("read from pipe error");
		exit(-1);
	    }
	    if (n == 0) {
		fprintf(stderr, "child closed pipe\n");
		break;
	    }
	    line[n] = 0;
	    if (fputs(line, stdout) == EOF) {
		perror("fputs error");
		exit(-1);
	    }
	}
	if (ferror(stdin)) {
	    perror("fgets error on stdin");
	    exit(-1);
	}
	exit(0);
    } else {
	close(fd1[1]);
	close(fd2[0]);

	if (fd1[0] != STDIN_FILENO) {
	    if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) {
		perror("dup2 error to stdin");
		exit(-1);
	    }
	}
	if (fd2[1] != STDOUT_FILENO) {
	    if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) {
		perror("dup2 error to stdout");
		exit(-1);
	    }
	}
	if (execl("add2", "add2", (char *)0) < 0) {
	    perror("execl error on add2");
	    exit(-1);
	}
	exit(0);
    }

    return 0;
}

static void sig_pipe(int signo) {
    printf("Caught SIGPIPE\n");
    exit(1);
}
