#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define FIFO_PATH "/tmp/fifodemo"
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define MAXLINE 1024

char *readline(int fd, char *buf, int size) {
    char c, *ptr = buf;
    int n;
    while ((n = read(fd, &c, 1)) > 0 && ptr < &buf[size-1]) {
	*ptr++ = c;
	if (c == '\n')
	    break;
    }
    if (n < 0) {
	perror("read error");
	exit(-1);
    }
    if (n == 0) {
	return NULL;
    }
    *ptr = 0;
    return buf;
}

int main(void) {
    pid_t pid;
    char buf[MAXLINE];

    if (mkfifo(FIFO_PATH, FILE_MODE) < 0) {
	perror("mkfifo error");
	exit(-1);
    }
    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid > 0) {
	int fifofd = open(FIFO_PATH, O_RDONLY);
	while (readline(fifofd, buf, MAXLINE) != NULL) {
	    printf("%s", buf);
	}
	printf("FIFO write end closed\n");
	exit(0);
    } else {
	int fifofd = open(FIFO_PATH, O_WRONLY);
	while (fgets(buf, MAXLINE, stdin) != NULL) {
	    int len = strlen(buf);
	    if (write(fifofd, buf, len) != len) {
		perror("write error");
		exit(-1);
	    }
	}
	close(fifofd);
	exit(0);
    }
    return 0;
}
