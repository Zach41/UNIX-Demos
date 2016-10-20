#include <stdio.h>

void pr_stdio(const char *name, FILE *fp);
int is_unbuffered(FILE *);
int is_linebuffered(FILE *);
int buffer_size(FILE *);

int main(int argc, char *argv[])
{
    FILE *fp;

    fputs("enter any character\n", stdout);
    // get a char so that a buffer is initialized for the stdio
    if (getchar() == EOF) {
	fprintf(stderr, "getchar error\n");
	return -1;
    }
    fputs("one line to stdandard error\n", stderr);

    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);

    if ((fp = fopen("/etc/passwd", "r")) == NULL) {
	fprintf(stderr, "fopen error\n");
	return -1;
    }
    if (getc(fp) == EOF) {
	fprintf(stderr, "getc error\n");
	return -1;
    }
    pr_stdio("/etc/passwd", fp);
    
    return 0;
}

void pr_stdio(const char *name, FILE *fp) {
    printf("stream = %s, ", name);
    if (is_unbuffered(fp)) {
	printf("unbuffered");
    } else if (is_linebuffered(fp)) {
	printf("line buffered");
    } else {
	printf("fully buffered");
    }
    printf(", buffer size = %d\n", buffer_size(fp));
}

// Linux using _IONBF
int is_unbuffered(FILE *fp) {
    return (fp -> _flags & _IONBF);
}

int is_linebuffered(FILE *fp) {
    return (fp -> _flags & _IOLBF);
}

int buffer_size(FILE *fp) {
    return fp -> _IO_buf_end - fp -> _IO_buf_base;
}

