#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <stdlib.h>

#define MAX_PASS_LEN    8

char *getpass(const char *prompt) {
    static char buf[MAX_PASS_LEN + 1];
    char *ptr;
    sigset_t sig, osig;
    struct termios ts, ots;
    FILE *fp;

    if ((fp = fopen(ctermid(NULL), "r+")) == NULL) {
	return NULL;
    }

    setbuf(fp, NULL);

    // block signals
    sigemptyset(&sig);
    sigaddset(&sig, SIGINT);
    sigaddset(&sig, SIGTSTP);
    sigprocmask(SIG_BLOCK, &sig, &osig);

    // set terminal
    if (tcgetattr(fileno(fp), &ts) < 0) {
	perror("tcgetattr error");
	exit(-1);
    }
    ots = ts;
    ts.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    tcsetattr(fileno(fp), TCSAFLUSH, &ts);

    fputs(prompt, fp);
    ptr = buf;
    char c;
    while ((c = getc(fp)) != EOF && c != '\n')
	if (ptr < &buf[MAX_PASS_LEN])
	    *ptr++ = c;
    *ptr = 0;
    putc('\n', fp);

    // restore terminal attributes
    tcsetattr(fileno(fp), TCSAFLUSH, &ots);
    // restore blocking signals
    sigprocmask(SIG_SETMASK, &osig, NULL);
    fclose(fp);

    return buf;
}

int main(void) {
    char *ptr;

    if ((ptr = getpass("Enter password:")) == NULL) {
	perror("getpass error");
	exit(-1);
    }
    printf("password: %s\n", ptr);

    while (*ptr) {
	*ptr++ = 0;
    }

    return 0;
}
