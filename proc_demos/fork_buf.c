#include <stdio.h>
#include <unistd.h>

int globalvar = 6;
char buf[] = "a write to stdout\n";

int main(int argc, char *argv[])
{
    int var;
    pid_t pid;

    var = 88;

    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) {
	perror("write error");
	return -1;
    }
    printf("before fork\n");

    if ((pid = fork()) < 0) {
	perror("fork error");
	return -1;
    } else if (pid == 0) {
	globalvar++;
	var++;
    } else {
	sleep(2);
    }

    printf("pid = %ld, globalvar = %d, var = %d\n", (long)pid, globalvar, var);
    
    return 0;
}
