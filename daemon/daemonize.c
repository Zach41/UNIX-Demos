#include <syslog.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

sigset_t mask;

void reread() {
    // re-read configuration
    
}

int lockfile(int fd) {
    struct flock fl;

    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_whence = SEEK_SET;
    fl.l_type = F_WRLCK;

    return fcntl(fd, F_SETLK, &fl);
}

void daemonize(const char *cmd) {
    int fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
	perror(" getrlimit error");
	exit(-1);
    }

    // become a session leader to lose controlling TTY
    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid > 0) {
	/* parent */
	exit(0);
    }
    setsid();

    /* fork again, ensure future opens won;t allocate controlling TTY */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
	fprintf(stderr, "%s: can't ignore SIGHUP", cmd);
	perror("");
	exit(-1);
    }

    if ((pid = fork()) < 0) {
	perror("fork error");
	exit(-1);
    } else if (pid > 0) {
	exit(0);
    }

    /* Change the current working directory to the root
       so we won;t prevent file systems from being unmounted.
       */
    if (chdir("/") < 0) {
	perror("chdir error");
	exit(-1);
    }

    /* close all open file descriptors */
    if (rl.rlim_max == RLIM_INFINITY) {
	rl.rlim_max = 1024;
    }
    for (int i=0; i<rl.rlim_max; i++) {
	close(i);
    }

    /* attach stdin, stdout, stderr to /dev/null */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /* Initialize log file */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
	syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
	exit(1);
    }
    
}

int already_running(void) {
    int fd;
    char buf[16];

    fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0) {
	syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
	exit(1);
    }

    if (lockfile(fd) < 0) {
	if (errno == EACCES || errno == EAGAIN) {
	    /* already has one daemon running */
	    close(fd);
	    return 1;
	}
	syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
	exit(1);
    }
    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    return 0;
}

void *thr_fn(void *arg) {
    int err, signo;
    for(;;) {
	err = sigwait(&mask, &signo);
	if (err != 0) {
	    syslog(LOG_ERR, "sigwait failed");
	    exit(1);
	}

	switch (signo) {
	case SIGHUP: {
	    syslog(LOG_INFO, "Re-reading configuration file");
	    reread();
	    break;
	}
	case SIGTERM: {
	    syslog(LOG_INFO, "got SIGTERM; exiting");
	    exit(0);
	}
	default:
	    syslog(LOG_INFO, "unexpected signal %d\n", signo);
	}
    }

    return 0;
}

int main(int argc, char **argv) {

    int err;
    pthread_t tid;
    char *cmd;
    struct sigaction sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL)
	cmd = argv[0];
    else
	cmd++;

    daemonize(cmd);

    if (already_running()) {
    	syslog(LOG_ERR, "daemon already running");
    	exit(1);
    }

    /*
      Restore SIGHUP default and block all signals
      */
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
    	syslog(LOG_ERR, "%s: can't restore SIGHUP default", cmd);
    	exit(1);
    }
    sigfillset(&mask);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0) {
    	syslog(LOG_ERR, "SIG_BLOCK error");
    	exit(1);
    }

    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0) {
    	syslog(LOG_ERR, "can't create thread");
    }
    
    /* do daemon's work */
    while (1);

    exit(0);
}
