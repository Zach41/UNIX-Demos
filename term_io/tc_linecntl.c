#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    char msg[2048];
    time_t t;
    
    if (isatty(STDIN_FILENO) == 0) {
	perror("stdin is not a terminal device");
	exit(-1);
    }
    if (tcflow(STDIN_FILENO, TCIOFF) < 0) {
	perror("tcflow (TCIOFF) failed");
	exit(-1);
    }

    /* while (scanf("%s", msg) != EOF) { */
    /* 	printf("%s\n", msg); */
    /* }  */
    return 0;       
}
