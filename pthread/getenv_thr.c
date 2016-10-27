#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "pthread_functions.h"

#define MAXSTRINGSZ 4096

static pthread_key_t key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
static pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char **environ;

static void thread_init(void) {
    pthread_key_create(&key, free);
}

char *getenv_(const char *name) {
    char *envbuf;
    int len;

    pthread_once(&init_done, thread_init);
    pthread_mutex_lock(&env_mutex); /* in case setenv changes the value during getting */
    envbuf = (char *)pthread_getspecific(key);
    if (envbuf == NULL) {
	envbuf = malloc(MAXSTRINGSZ);
	/* printf("malloc, address: %p\n", envbuf); */
	if (envbuf == NULL) {
	    pthread_mutex_unlock(&env_mutex);
	    return NULL;
	}
	pthread_setspecific(key, envbuf);
    }
    len = strlen(name);
    for (int i=0; environ[i] != 0; i++) {
	if (strncmp(name, environ[i], len) == 0 && environ[i][len] == '=') {
	    strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ - 1);
	    pthread_mutex_unlock(&env_mutex);
	    return envbuf;
	}
    }
    pthread_mutex_unlock(&env_mutex);
    return NULL;    
}
