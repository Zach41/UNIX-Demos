#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(void) {
    struct termios term;
    long vdisable;

    if (isatty(STDIN_FILENO) == 0) {
	perror("stdin is not a terminal device");
	exit(-1);
    }
    if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0) {
	perror("fpathconf error or _POSIX_VDISABLE not in effect");
	exit(-1);
    }
    if (tcgetattr(STDIN_FILENO, &term) < 0) {
	perror("tcgetattr error");
	exit(-1);
    }
    
    term.c_cc[VINTR] = _POSIX_VDISABLE;
    term.c_cc[VEOF] = 2;	/* ASCII code */

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0) {
	perror("tcsetatr error");
	exit(-1);
    }
    return 0;
}
