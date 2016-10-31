#ifndef SEM_LOCK_H
#define SEM_LOCK_H

#include <limits.h>
#include <semaphore.h>

struct slock {
    sem_t *semp;
    char name[_POSIX_NAME_MAX];
};

struct slock *s_alloc();

void s_free(struct slock *sp);

int s_lock(struct slock *sp);

int s_trylock(struct slock *sp);

int s_unlock(struct slock *sp);

#endif /* SEM_LOCK_H */
