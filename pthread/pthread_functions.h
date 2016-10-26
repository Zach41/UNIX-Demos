#ifndef PTHREAD_FUNCTIONS_H
#define PTHREAD_FUNCTIONS_H

int makethread(void *(*fn)(void *), void *arg, pthread_t *tidp);

#endif /* PTHREAD_FUNCTIONS_H */
