#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void pr_winsz(int fd) {
    struct winsize size;

    if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0) {
	perror("ioctl error");
	exit(-1);
    }
    printf("%d row, %d columns\n", size.ws_row, size.ws_col);
}

static void sig_winch(int signo) {
    printf("SIGWINCH received\n");
    pr_winsz(STDIN_FILENO);
}

int main(void) {
    if (isatty(STDIN_FILENO) == 0)
	exit(1);
    if (signal(SIGWINCH, sig_winch) == SIG_ERR) {
	perror("signal error");
	exit(-1);
    }
    pr_winsz(STDIN_FILENO);
    for (;;)
	pause();

    return 0;
}
