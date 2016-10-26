#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
struct job
{
    int j_id;
    struct job *j_prev;
    struct job *j_next;
};

struct queue
{
    pthread_rwlock_t q_lock;
    struct job *q_head;
    struct job *q_tail;
};

int queue_init(struct queue *qp) {

    int err;

    qp -> q_head = NULL;
    qp -> q_tail = NULL;
    err = pthread_rwlock_init(&qp -> q_lock, NULL);
    if (err)
	return err;
    return 0;
}

void delay(int sec) {
    time_t t;
    for (t = time(NULL); t+sec > time(NULL); )
	;
}

void job_insert(struct queue *qp, struct job *jp) {
    // insert a job at head
    pthread_rwlock_wrlock(&qp -> q_lock);
    jp -> j_next = qp -> q_head;
    jp -> j_prev = NULL;
    if (qp -> q_head != NULL) {
	qp -> q_head -> j_prev = jp;
    } else {
	// empty list
	qp -> q_tail = jp;
    }
    qp -> q_head = jp;
    pthread_rwlock_unlock(&qp -> q_lock);
}

void job_append(struct queue *qp, struct job *jp) {
    // append a job to the list
    pthread_rwlock_wrlock(&qp -> q_lock);
    jp -> j_prev = qp -> q_tail;
    jp -> j_next = NULL;
    if (qp -> q_tail != NULL) {
	qp -> q_tail -> j_next = jp;
    } else {
	qp -> q_head = jp;
    }
    qp -> q_tail = jp;
    pthread_rwlock_unlock(&qp -> q_lock);
}

void job_remove(struct queue *qp, struct job *jp) {
    pthread_rwlock_wrlock(&qp -> q_lock);
    if (jp == qp -> q_head) {
	qp -> q_head = jp -> j_next;
	if (qp -> q_tail == jp) {
	    qp -> q_tail = NULL;
	} else {
	    jp -> j_next -> j_prev = NULL;
	}
    } else if (jp == qp -> q_tail) {
	qp -> q_tail = jp -> j_prev;
	if (jp == qp -> q_head) {
	    qp -> q_head = NULL;
	} else {
	    jp -> j_prev -> j_next = NULL;
	}	
    } else {
	jp -> j_prev -> j_next = jp -> j_next;
	jp -> j_next -> j_prev = jp -> j_prev;
    }
    pthread_rwlock_unlock(&qp -> q_lock);
}

struct job *job_find(struct queue *qp, int id) {
    struct job *jp;
    if (pthread_rwlock_rdlock(&qp -> q_lock) != 0)
	return NULL;

    for (jp = qp -> q_head; jp != NULL; jp = jp -> j_next) {
	if (jp -> j_id == id)
	    break;
    }
    pthread_rwlock_unlock(&qp -> q_lock);
    return jp;
}

void traverse_jobs(struct queue *qp) {
    struct job *jp;
    int err;
    if ((err = pthread_rwlock_rdlock(&qp -> q_lock)) != 0) {
	fprintf(stderr, "pthread_rwlock_rdlock error: %s\n", strerror(err));
	exit(-1);
    }
    printf("delay before traversing\n");
    delay(5);
    printf("Queue:");
    for (jp = qp -> q_head; jp != NULL; jp = jp-> j_next) {
	printf(" %d", jp -> j_id);
    }
    printf("\n");
    pthread_rwlock_unlock(&qp -> q_lock);
}

#define NJOBS 20
struct queue *qp;
struct job* jp_arr[NJOBS];

void *thr_fn1(void *arg) {
    for (int i=0; i<NJOBS; i++) {
	printf("inserting\n");
	struct job *jp = malloc(sizeof(struct job));
	jp -> j_id = i;
	if (i%2)
	    job_insert(qp, jp);
	else
	    job_append(qp, jp);
	jp_arr[i] = jp;
	delay(1);
    }

    for (int i=0; i<NJOBS; i++) {
	printf("deleting\n");
    	job_remove(qp, jp_arr[i]);
	delay(1);
    }
    return (void *)0;    
}

void *thr_fn2(void *arg) {
    sleep(5);
    while (qp -> q_head != qp -> q_tail) {
	traverse_jobs(qp);
	delay(3);
    }
    printf("Done\n");
    return (void *)0;
}

int main(void) {
    int err;
    pthread_t tid1, tid2;
    void *tret;

    setbuf(stdout, NULL);
    
    qp = (struct queue *)malloc(sizeof(struct queue));
    err = queue_init(qp);
    if (err != 0) {
	fprintf(stderr, "error: %s\n", strerror(err));
	exit(-1);
    }
    
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0) {
	fprintf(stderr, "error: %s\n", strerror(err));
	exit(-1);
    }
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0) {
	fprintf(stderr, "error: %s\n", strerror(err));
	exit(-1);
    }
    err = pthread_join(tid1, &tret);
    err = pthread_join(tid2, &tret);

    return 0;
}

