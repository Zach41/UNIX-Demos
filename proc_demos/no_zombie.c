#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void) {
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
	perror("fork error");
	return -1;
    } else if (pid == 0) {
	if ((pid = fork()) < 0) {
	    perror("fork error");
	    exit(-1);
	} else if (pid > 0) {
	    exit(0);
	}

	sleep(5);
	printf("parent process id: %ld\n", (long)getppid());
	exit(0);
    }

    if (waitpid(pid, NULL, 0) != pid) {
	perror("wait error");
	return -1;
    }

    return 0;

}
