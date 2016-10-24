#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    time_t t;
    struct tm *tmptr;

    char buf1[16];
    char buf2[64];

    time(&t);
    tmptr = localtime(&t);
    if (strftime(buf1, 16, "time and date: %r, %a %b %d %Y", tmptr) == 0) {
	printf("buffer length 16 is too small\n");
    } else
	printf("%s\n", buf1);

    if (strftime(buf2, 64, "time and date: %r, %a %b %d %Y", tmptr) == 0) {
	printf("buffer length 64 is too small\n");
    } else
	printf("%s\n", buf2);

    return 0;
}