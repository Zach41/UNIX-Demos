#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define IPADDRESS    "127.0.0.1"
#define PORT         8080
#define MAXSIZE      1024
#define LISTENQ      20
#define EPOLLEVENTS  100

static int socket_bind(const char* ip, int port);
static void do_epoll(int listenfd);
static void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf);
static void handle_accept(int epollfd, listenfd);    
static void do_read(int epollfd, int fd, char *buf);
static void do_write(int epollfd, int fd, char *buf);
static void add_event(int epollfd, int fd, int state);
static void modify_event(int epollfd, int fd, int state);
static void delete_event(int epollfd, int fd, int state);

int main(void) {
    int listenfd = socket_bind(IPADDRESS, PORT);
    listen(listenfd, LISTENQ);
    do_epoll(listenfd);
    return 0;
}

static int socket_bind(const char *ip, int port) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
	perror("socket create error");
	exit(-1);
    }
    
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = INADDR_ANY;

    if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
	perror("bind socket error");
	exit(-1);
    }
    return listenfd;
}

static void do_epoll(int listenfd) {
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    char buf[MAXSIZE];
    memset(buf, 0, sizeof(buf));

    epollfd = epoll_create(FDSIZE); /* create a epoll fd which can handles FDSIZE fds */
    if (epollfd < 0) {
	perror("create epoll error");
	exit(-1);
    }
    add_event(epoll_fd, listenfd, EPOLLIN);

    int ret;
    for (;;) {
	ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
	if (ret < 0) {
	    perror("epoll wait error");
	    exit(-1);
	}
	handle_events(epollfd, events, ret, listenfd, buf);
    }
    close(epollfd);		/* must close epoll fd */    
}

static void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf) {
    int fd;
    for (i = 0; i<num; i++) {
	fd = events[i].data.fd;
	if (fd == listenfd && (events[i].events & EPOLLIN))
	    handle_accept(epollfd, listenfd);
	else if (events[i].events & EPOLLIN)
	    do_read(epollfd, fd, buf);
	else if (events[i].events & EPOLLOUT)
	    do_write(epollfd, fd, buf);
    }
}

static void handle_accept(int epollfd, int listenfd) {
    struct sockaddr_in cli_addr;
    int cli_size = 0;
    int clifd;

    if ((clifd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_size)) < 0) {
	perror("accept error");
    } else {
	printf("accept a new client: %s:%d\n", inet_ntop(cli_addr.sin_addr), cli_addr.sin_port);
	add_event(epollfd, clifd, EPOLLIN);
    }    
}

static void do_read(int epollfd, int fd, char *buf) {
    int nread = read(fd, buf, MAXSIZE);
    if (nread == -1) {
	perror("read error");
	close(fd);
	delete_event(epollfd, fd, EPOLLIN);
    } else if (nread == 0) {
	fprintf(stderr, "client close.\n");
	close(fd);
	delete_event(epollfd, fd, EPOLLIN);
    } else {
	printf("read message: %s", buf);
	modify_event(epollfd, fd, EPOLLOUT);
    }
}

static void do_write(int epollfd, int fd, char *buf) {
    int nwrite = write(fd, buf, strlen(buf));
    if (nwrite == -1) {
	perror("write error");
	close(fd);
	delete_event(epollfd, fd, EPOLLOUT);
    } else
	modify_event(epollfd, fd, EPOLLIN);
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
