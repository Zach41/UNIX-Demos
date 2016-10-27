#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "flock_helpers.h"
#include "tell_wait.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IXUSR)

static void lockabyte(const char *name, int fd, off_t offset) {
    if (writew_lock(fd, offset, SEEK_SET, 1) < 0) {
	fprintf(stderr, "writew_lock error: %s\n", strerror(errno));
	exit(-1);
    }
    printf("%s: got the lock, byte %lld\n", name, (long long)offset);
}

int main(void) {
    int fd;
    pid_t pid;

    if ((fd = creat("tmplock", FILE_MODE)) < 0) {
	fprintf(stderr, "creat error: %s\n", strerror(errno));
	exit(-1);
    }
    if (write(fd, "ab", 2) != 2) {
	fprintf(stderr, "write error: %s\n", strerror(errno));
	exit(-1);
    }

    TELL_WAIT();
    if ((pid = fork()) < 0) {
	fprintf(stderr, "fork error: %s\n", strerror(errno));
	exit(-1);
    } else if (pid > 0) {
	lockabyte("parent", fd, 1);
	TELL_CHILD(pid);
	WAIT_CHILD();
	lockabyte("parent", fd, 0);
    } else {
	lockabyte("child", fd, 0);
	TELL_PARENT(pid);
	WAIT_PARENT();
	lockabyte("child", fd, 1);
    }
    return 0;
}
