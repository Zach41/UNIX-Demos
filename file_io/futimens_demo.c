#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int fd;
    struct stat sbuf;
    struct timeval tv[2];
    
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <pathname> [pathname ...]\n", argv[0]);
	exit(1);
    }

    for (int i=1; i<argc; i++) {
	if (stat(argv[i], &sbuf) < 0) {
	    fprintf(stderr, "stat error on %s: %s\n", argv[i], strerror(errno));
	    continue;
	}
	if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {
	    fprintf(stderr, "open error on %s: %s\n", argv[i], strerror(errno));
	    continue;
	}
	tv[0].tv_sec = sbuf.st_atimespec.tv_sec;
	tv[0].tv_usec = sbuf.st_atimespec.tv_nsec / 1000;
	tv[1].tv_sec = sbuf.st_mtimespec.tv_sec;
	tv[1].tv_usec = sbuf.st_mtimespec.tv_nsec / 1000;

	if (futimes(fd, tv) < 0) {
	    fprintf(stderr, "futimens error on %s: %s\n", argv[i], strerror(errno));
	}
	close(fd);	
    }

    return 0;
}
