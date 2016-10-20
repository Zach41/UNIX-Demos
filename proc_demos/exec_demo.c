#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

char *env_init[] = {"USER=unkown", NULL};

int main(int argc, char *argv[])
{
    pid_t pid;
    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid == 0) {
	if (execle("/home/zach41/Desktop/UNIX-Demos/proc_demos/echoall", "echoall", "myarg1", "MY ARG2", (char *)0, env_init) < 0) {
	    perror("execle error");
	    exit(-1);
	}
    }
    if (waitpid(pid, NULL, 0) < 0) {
	perror("waitpid error");
	exit(-1);
    }

    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid == 0) {
	if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0) {
	    perror("execlp error");
	    exit(-1);
	}
    }

    if (waitpid(pid, NULL, 0) < 0) {
	perror("waitpid error");
	exit(-1);
    }
    return 0;
}
