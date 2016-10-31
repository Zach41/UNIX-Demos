#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define COPYINCR (1024*1024*1024)

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char *argv[])
{
    int fdin, fdout;
    void *src, *dst;
    struct stat sbuf;
    off_t fsz = 0;

    if (argc != 3) {
	fprintf(stderr, "Usage: %s <srcfile> <dstfile>\n", argv[0]);
	exit(127);
    }

    if ((fdin = open(argv[1], O_RDONLY)) < 0) {
	perror("open error");
	exit(-1);
    }

    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
	perror("open error");
	exit(-1);
    }

    if (fstat(fdin, &sbuf) < 0) {
	perror("fstat error");
	exit(-1);
    }

    if (ftruncate(fdout, sbuf.st_size) < 0) {
	perror("ftruncate error");
	exit(-1);
    }
    size_t copysz;
    // map file to memory
    while (fsz < sbuf.st_size) {
	if (sbuf.st_size - fsz > COPYINCR)
	    copysz = COPYINCR;
	else
	    copysz = sbuf.st_size - fsz;

	if ((src = mmap(0, copysz, PROT_READ, MAP_SHARED, fdin, fsz)) == MAP_FAILED) {
	    perror("mmap error");
	    exit(-1);
	}
	if ((dst = mmap(0, copysz, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, fsz)) == MAP_FAILED) {
	    perror("mmap error");
	    exit(-1);
	}
	memcpy(dst, src, copysz);
	munmap(src, copysz);
	munmap(dst, copysz);
	fsz += copysz;
    }
    return 0;
}

