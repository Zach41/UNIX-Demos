#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

struct foo
{
    int                f_count;
    pthread_mutex_t    f_lock;
    int                f_id;
};

struct foo *foo_alloc(int id) {
    struct foo *fp;
    if ((fp = malloc(sizeof(struct foo))) != NULL) {
	fp -> f_count = 1;
	if (pthread_mutex_init(&fp -> f_lock, NULL) != 0) {
	    free(fp);
	    return NULL;
	}
	fp -> f_id = id;
    }
    return fp;
}

void foo_hold(struct foo *fp) {
    pthread_mutex_lock(&fp -> f_lock);
    printf("[foo] id: %d, count: %d\n", fp->f_id, fp->f_count);
    fp -> f_count += 1;
    pthread_mutex_unlock(&fp -> f_lock);
}

void foo_rele(struct foo *fp) {
    pthread_mutex_lock(&fp -> f_lock);
    if (--fp -> f_count == 0) {
	printf("Deinitialize foo (id :%d)\n", fp -> f_id);
	pthread_mutex_unlock(&fp->f_lock);
	pthread_mutex_destroy(&fp->f_lock);
	free(fp);
    } else 
    pthread_mutex_unlock(&fp->f_lock);
}

struct foo *foo_val1;
struct foo *foo_val2;

void *thr_fn1(void *arg) {
    for (int i=0; i<20; i++) {
	foo_hold(foo_val1);
    }

    for (int i=0; i<22; i++) {
	foo_hold(foo_val2);
    }

    for (int i=0; i<20; i++) {
	foo_rele(foo_val1);
    }

    for (int i=0; i<22; i++) {
	foo_rele(foo_val2);
    }

    return (void *)0;
}

void *thr_fn2(void *arg) {
    for (int i=0; i<20; i++) {
	foo_hold(foo_val1);
    }

    for (int i=0; i<22; i++) {
	foo_hold(foo_val2);
    }

    for (int i=0; i<21; i++) {
	foo_rele(foo_val1);
    }

    for (int i=0; i<23; i++) {
	foo_rele(foo_val2);
    }

    return (void *)0;
}

int main(void) {
    int err;
    pthread_t tid1, tid2;
    void *tret;

    foo_val1 = foo_alloc(1);
    foo_val2 = foo_alloc(2);

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);

    err = pthread_join(tid1, &tret);
    err = pthread_join(tid2, &tret);

    return 0;
}

