#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define DEF_PAGER    "/usr/bin/more"
#define MAXLINE      2048

int main(int argc, char *argv[])
{
    int fd[2];
    pid_t pid;
    FILE *fp;
    char line[MAXLINE];
    
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <pathname>", argv[0]);
	exit(-1);
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
	perror("fopen error");
	exit(-1);
    }
    if (pipe(fd) < 0) {
	perror("pipe error");
	exit(-1);
    }

    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid > 0) {
	close(fd[0]);
	int n;
	while (fgets(line, MAXLINE, fp) != NULL) {
	    n = strlen(line);
	    if (write(fd[1], line, n) != n) {
		perror("write error");
		exit(-1);
	    }
	}
	if (ferror(fp)) {
	    perror("fgets error");
	    exit(-1);
	}
	close(fd[1]);

	if (waitpid(pid, NULL, 0) < 0) {
	    perror("waitpid error");
	    exit(-1);
	}
	exit (0);
    } else {
	close(fd[1]);
	if (fd[0] != STDIN_FILENO) {
	    if (dup2(fd[0], STDIN_FILENO) < 0) {
		perror("dup2 error");
		exit(-1);
	    }
	    close(fd[0]);
	}
	char *argv0;
	char *pager = DEF_PAGER;
	if ((argv0 = strrchr(pager, '/')) != NULL)
	    argv0++;
	else
	    argv0 = pager;
	if (execl(pager, argv0, (char *)0) < 0) {
	    perror("execl error");
	    exit(-1);
	}
    }
    return 0;
}
