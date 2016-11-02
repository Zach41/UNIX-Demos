#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct stat sbuf;
    char *ptr;

    for (int i=1; i<argc; i++) {
	printf("%s: ", argv[i]);

	if (lstat(argv[i], &sbuf) < 0) {
	    perror("lstat error");
	    exit(-1);
	}

	if (S_ISREG(sbuf.st_mode))
	    ptr = "regular file";
	else if (S_ISDIR(sbuf.st_mode))
	    ptr = "diretory";
	else if (S_ISCHR(sbuf.st_mode))
	    ptr = "character special";
	else if (S_ISBLK(sbuf.st_mode))
	    ptr = "block special";
	else if (S_ISFIFO(sbuf.st_mode))
	    ptr = "fifo";
	else if (S_ISLNK(sbuf.st_mode))
	    ptr = "symbolic link";
	else if (S_ISSOCK(sbuf.st_mode))
	    ptr = "socket";
	else
	    ptr = "** unknown mode **";
	printf("%s\n", ptr);
    }
    return 0;
}
