#ifndef SIGNAL_FUNCTIONS_H
#define SIGNAL_FUNCTIONS_H

void print_sigset(FILE *, const char *, const sigset_t *);
int print_sigmask(FILE *, const char *);
int print_pendingsigs(FILE *, const char *);

#endif /* SIGNAL_FUNCTIONS_H */
