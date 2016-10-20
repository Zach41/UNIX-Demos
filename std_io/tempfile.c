#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void make_temp(char *template);

int main(int argc, char *argv[])
{
    char good_template[] = "/tmp/dirXXXXXX";
    char *bad_template = "tmp/dirXXXXXX";

    printf("trying to create first temp file...\n");
    make_temp(good_template);
    printf("trying to create second temp file...\n");
    make_temp(bad_template);

    return 0;
}

void make_temp(char *template) {
    int fd;
    struct stat sbuf;

    if ((fd = mkstemp(template)) == -1) {
	perror("create temp file failed");
	exit(-1);
    }
    printf("temp name = %s\n", template);
    if (stat(template, &sbuf) == -1) {
	if (errno == ENOENT) {
	    printf("file doesn't exist\n");	    
	} else {
	    printf("stat error\n");
	    exit(-1);
	}
    }
    printf("file exists\n");
    unlink(template);    
}
