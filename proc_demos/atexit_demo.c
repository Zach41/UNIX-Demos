 #include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

static void clear_onexit1() {
    printf("On exit1\n");
}

static void clear_onexit2() {
    printf("On exit2\n");
}

int main(void) {
    printf("At Exit Demo!\n");

    if (atexit(clear_onexit1) != 0) {
	fprintf(stderr, "can't register `clear_onexit1`");
	return -1;
    }
    if (atexit(clear_onexit1) != 0) {
	fprintf(stderr, "can't register `clear_onexit1`");
	return -1;
    }
    if (atexit(clear_onexit2) != 0) {
	fprintf(stderr, "can't register `clear_onexit1`");
	return -1;
    }
    exit(0);
    /* _exit(0); */
}
