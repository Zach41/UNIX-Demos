#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());

    return 0;
}
