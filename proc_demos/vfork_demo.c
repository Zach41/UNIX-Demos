#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int globvar = 6;

int main(int argc, char *argv[])
{
    int var;
    pid_t pid;

    var = 88;
    
    printf("before vfork\n");
    if ((pid = vfork()) < 0) {
	perror("vfork error");
	return -1;
    } else if (pid == 0) {
	// now child process is running in parent address space!!!
	globvar++;
	var++;
	_exit(0);
	/* exit(0); */
    }
    printf("pid = %ld, glob = %d, var = %d\n", (long)pid, globvar, var);
    return 0;
}
