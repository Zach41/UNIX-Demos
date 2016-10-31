#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PAGER    "${PAGER:-more}"
#define MAXLINE  2048

int main(int argc, char *argv[])
{
    FILE *fpin, *fpout;
    char line[MAXLINE];
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <pathname>", argv[0]);
	exit(127);
    }

    if ((fpin = fopen(argv[1], "r")) == NULL) {
	perror("fopen error");
	exit(-1);
    }

    if ((fpout = popen(PAGER, "w")) == NULL) {
	perror("popen error");
	exit(-1);
    }
    
    while (fgets(line, MAXLINE, fpin) != NULL) {
	fputs(line, fpout);
    }

    if (ferror(fpin)) {
	perror("fgets error");
	exit(-1);
    }
    if (pclose(fpout) == -1) {
	perror("pclose error");
	exit(-1);
    }
    
    return 0;
}
