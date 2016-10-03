#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>

#define IPADDRESS    "127.0.0.1"
#define PORT         8080
#define MAXSIZE      1024
#define LISTENQ      20
#define EPOLLEVENTS  100

static void handle_connection(int sockfd);
static void handle_events(int pollfd, struct epoll_event *events, int num, int sockfd, char *buf);
static do_read(int epollfd, int fd, int sockfd, char *buf);
static do_write(int epollfd, int fd, int sockfd, char *buf);
static void add_event(int epollfd, int fd, int state);
static void modify_event(int epollfd, int fd, int state);
static void delete_event(int epollfd, int fd, int state);

int conn_flag;

int main(void) {
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
	perror("socket create error");
	exit(-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(PORT);
    if (inet_pton(AF_INET, IPADDRESS, &serv_addr.sin_addr) != 1) {
	perror("inet_pton error");
	exit(-1);
    }

    handle_connection(sockfd);
    close(sockfd);
    return 0;
}

static handle_connection(int sockfd) {
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    char buf[MAXSIZE];

    epollfd = epoll_create(FDSIZE);
    if (epollfd < 0) {
	perror("create epoll error");
	exit(-1);
    }
    add_event(epollfd, sockfd, EPOLLIN);

    int ret;
    for (;;) {
	ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
	handle_events(epollfd, events, ret, sockfd, buf);
    }
}

static void handle_events(int epollfd, struct epoll_event *events, int num, int sockfd, char *buf) {
    int fd;
    for (int i=0; i<num; i++) {
	fd = events[i].data.fd;
	if (fd == sockfd && events[i].events & EPOLLIN) {
	    if (!conn_flag) {
		conn_flag = 1;
		add_event(epollfd, STDIN_FILENO, EPOLLIN);
	    } else {
		do_read(epollfd, sockfd, sockfd, buf);
	    }	 
	} else if (fd == STDIN_FILENO && events[i].events & EPOLLIN) {
	    do_read(epollfd, fd, sockfd, buf);
	} else if (fd == STDOUT_FILENO && events[i].events & EPOLLOUT) {
	    do_write(epollfd, fd, sockfd, buf);
	} else if (fd == sockfd && events[i].events & EPOLLOUT) {
	    do_write(epollfd, sockfd, sockfd, buf);		
	}
    }
}

static void do_read(int epollfd, int fd, int sockfd, char *buf) {
    int nread = read(fd, buf, MAXSIZE);
    if (nread < 0) {
	perror("read error");
	exit(-1);
    } else if (nread == 0) {
	fprintf(stderr, "server close.\nbye\n");
	exit(-1);
    } else {
	if (fd == STDIN_FILENO) {
	    add_event(epollfd, sockfd, EPOLLOUT);
	} else {
	    printf("message from server: %s\n", buf);
	    modify_event(epollfd, fd, EPOLLOUT);
	}
    }
}

static void do_write(int epollfd, int fd, int sockfd, char *buf) {
    int nwrite = write(fd, buf, strlen(buf));
    if (nwrite < 0) {
	perror("write error");
	exit(-1);
    } else {
	if (fd == STDOUT_FILENO) {
	    delete_event(epollfd, fd, EPOLLOUT);	    
	} else {
	    modify_event(epollfd, sockfd, EPOLLOUT);
	}
    }
    memset(buf, 0, MAXSIZE);
}

static void add_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

static void delete_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = state;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

static void modify_event(int epollfd, int fd, int state) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = state;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

