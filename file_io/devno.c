#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    struct stat sbuf;

    for (int i=1; i<argc; i++) {
	printf("%s: ", argv[i]);
	if (stat(argv[i], &sbuf) < 0) {
	    fprintf(stderr, "stat error on %s: %s\n", argv[i], strerror(errno));
	    continue;
	}
	printf("dev = %d/%d", major(sbuf.st_dev), minor(sbuf.st_dev));
	if (S_ISCHR(sbuf.st_mode) || S_ISBLK(sbuf.st_mode)) {
	    printf(" (%s) rdev = %d/%d", (S_ISCHR(sbuf.st_mode)) ? "character" : "block",
					  major(sbuf.st_rdev), minor(sbuf.st_rdev));
	}
	putchar('\n');
    }
    
    return 0;
}


