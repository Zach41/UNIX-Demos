#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
	exit(127);
    }

    if (access(argv[1], R_OK) < 0) {
	fprintf(stderr, "access error for %s: %s\n", argv[1], strerror(errno));
    } else {
	printf("read access OK\n");
    }

    if (open(argv[1], O_RDONLY) < 0) {
	fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
    } else {
	printf("open for reading OK\n");
    }

    return 0;
}
