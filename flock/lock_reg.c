#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "flock_helpers.h"
/*
  struct flock {
     short l_type;    // F_RDLCK, F_WRLCK, F_UNLCK
     short l_whence;  // SEEK_SET, SEEK_CUR, SEEK_END
     off_t l_start;  
     off_t l_len;
     pid_t l_pid;
  }
  */
int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) {
    struct flock fl;

    fl.l_type = type;
    fl.l_start = offset;
    fl.l_whence = whence;
    fl.l_len = len;

    return fcntl(fd, cmd, &fl);
}

/* if yes return 0, else return the pid of process which holds the lock */
int lock_test(int fd, int type, off_t offset, int whence, off_t len) {
    struct flock fl;

    fl.l_type = type;
    fl.l_whence = whence;
    fl.l_start = offset;
    fl.l_len = len;

    if (fcntl(fd, F_GETLK, &fl) < 0) {
	fprintf(stderr, "fcntl error: %s\n", strerror(errno));
	exit(-1);
    }
    
    if(fl.l_type == F_UNLCK)
	return 0;
    else
	return fl.l_pid;
}
