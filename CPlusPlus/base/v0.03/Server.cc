#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include "Server.h"

TcpServer::~TcpServer()
{
	close(listenfd_);
	close(epollfd_);
}
void TcpServer::Run()
{
	int connfd = 0;
	struct epoll_event ev;
	struct epoll_event events[1024];
	struct sockaddr_in cliaddr; // cli addr
	socklen_t len = sizeof(cliaddr);
	listenfd_ = createSocketAndListen(true);
	epollfd_ = epoll_create(10);
	if (epollfd_ < 0) {
		fprintf(stderr, "Create epollfd Error!\n");
		exit (-1);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listenfd_;
	if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, listenfd_, &ev) < 0) {
		fprintf(stderr, "epoll_ctl add listenfd Error!\n");
		exit(-1);
	}

	while(1) {
		int nfds = epoll_wait(epollfd_, events, 1024, -1);
		if (nfds == -1) {
			fprintf(stderr, "epoll_wait error!");
			exit(-1);
		}
		for (int i = 0; i < nfds; ++i) {
			if (events[i].data.fd == listenfd_) {
				connfd = accept(listenfd_, (struct sockaddr*)&cliaddr, &len);
				if (connfd < 0) {
					continue;
				}
				int ret = fcntl(connfd, F_GETFL);
				ret |= O_NONBLOCK;
				fcntl(connfd, F_SETFL, ret);
				ev.events = EPOLLIN;
				ev.data.fd = connfd;
				if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, connfd, &ev) == -1) {
					fprintf(stderr, "epoll_ctl add connfd error!");
				}
			} else {
				if (events[i].events & EPOLLIN) {
					char buf[1024] = {0};
					int ret = read(events[i].data.fd, buf, 1024);
					fprintf(stderr, "read: %s\n", buf);
					ret = write(events[i].data.fd, buf, strlen(buf));
					if (strncasecmp(buf, "Done", strlen(buf)) == 0) {
						close(connfd);
					}
				}
			}
		}
	}
}

int TcpServer::createSocketAndListen(bool nonblocking)
{
	int listenfd;
	socklen_t len;
	struct sockaddr_in srvaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		fprintf(stderr, "socket error!\n");
		return -1;
	}
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(8080);
	len = sizeof(srvaddr);
	if (bind(listenfd, (struct sockaddr*)&srvaddr, len) < 0) {
		fprintf(stderr, "socket bind error!\n");
		return -1;
	}
	int ret = listen(listenfd, 100);
	if (listenfd < 0) {
		fprintf(stderr, "listen error\n");
		return -1;
	}
	if (nonblocking) {
		if (setNonblock(listenfd) < 0) {
			fprintf(stderr, "listenfd set nonblocking error!");
			return -1;
		}
	}
	return listenfd;
}

int TcpServer::setNonblock(int fd)
{
	int ret;
	if ((ret = fcntl(fd, F_GETFL)) < 0) {
		fprintf(stderr, "fnctl F_GETFL error!");
		return -1;
	}
	ret |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, ret) < 0) {
		fprintf(stderr, "fcntl F_SETFL NONBLOCK errot!");
		return -1;
	}
	return 0;
}

