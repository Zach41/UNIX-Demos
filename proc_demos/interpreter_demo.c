#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid == 0) {
	if (execl("./testinterp", "testinterp", "myarg1", "MY ARG2", (char *)0) < 0) {
	    perror("execl error");
	    exit(-1);
	}
    }
    if (waitpid(pid, NULL, 0) < 0) {
	perror("waitpid error");
	exit(-1);
    }
    return 0;
}
