#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int val;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s fd\n", argv[0]);
	exit(127);
    }

    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
	perror("fcntl error");
	exit(-1);
    }
    switch (val & O_ACCMODE) {
    case O_RDONLY: {
	printf("read only");
	break;
    }
    case O_WRONLY:
	printf("write only");
	break;
    case O_RDWR:
	printf("read write");
	break;	
    default:
	printf("unknown access mode");
    }

    if (val & O_APPEND)
	printf(", append");
    if (val & O_NONBLOCK)
	printf(", nonblocking");
    if (val & O_SYNC)
	printf(", synchronous writes (data & properties)");
    if (val & O_DSYNC)
	printf(", synchronous writes (data only)");
    if (val & O_RSYNC)
	printf(", synchronous reads and write");
    putchar('\n');
    return 0;
}
