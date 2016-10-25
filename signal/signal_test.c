#include <stdio.h>
#include <signal.h>

#include "signal_functions.h"

int main(void) {
    sigset_t sigset;
    sigfillset(&sigset);

    print_sigset(stdout, "\t\t", &sigset);
}
