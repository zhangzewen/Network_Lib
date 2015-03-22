#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/fcntl.h>
#include "Server.h"
#include "Channel.h"

TcpServer::~TcpServer()
{
	close(listenfd_);
	close(epollfd_);
}
void TcpServer::Run()
{
	int connfd = 0;
	struct epoll_event ev;
	listenfd_ = createSocketAndListen(true);
	epollfd_ = epoll_create(10);
	if (epollfd_ < 0) {
		fprintf(stderr, "Create epollfd Error!\n");
		exit (-1);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listenfd_;
	Channel* channel = new Channel(epollfd_, listenfd_);
	channel->setCallBack(this);
	ev.data.ptr = static_cast<void*>(channel);
	if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, listenfd_, &ev) < 0) {
		fprintf(stderr, "epoll_ctl add listenfd Error!\n");
		exit(-1);
	}

	while(1) {
		int nfds = epoll_wait(epollfd_, events_, 1024, -1);
		if (nfds == -1) {
			fprintf(stderr, "epoll_wait error!");
			exit(-1);
		}
		for (int i = 0; i < nfds; ++i) {
			Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
			channel->handleEvent();
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

void TcpServer::callBack(int sockfd) {
	int connfd = -1;
	struct sockaddr_in cliaddr; // cli addr
	struct epoll_event ev;
	socklen_t len = sizeof(cliaddr);
	if (sockfd == listenfd_) {
		connfd = accept(listenfd_, (struct sockaddr*)&cliaddr, &len);
		if (connfd < 0) {
			return;
		}
		if (setNonblock(connfd) < 0) {
			fprintf(stderr, "connection set nonblocking error!");
			return;
		}
		
		Channel* channel = new Channel(epollfd_, connfd);
		channel->setCallBack(this);
		if (NULL == channel) {
			fprintf(stderr, "Create Channel error!\n");
			return ;
		}
		ev.events = EPOLLIN;
		ev.data.fd = connfd;
		ev.data.ptr = static_cast<void*>(channel);
		if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, connfd, &ev) == -1) {
			fprintf(stderr, "epoll_ctl add connfd error!");
		}
	} else {
		if (sockfd & EPOLLIN) {
			char buf[1024] = {0};
			int ret = read(sockfd, buf, 1024);
			fprintf(stderr, "read: %s\n", buf);
			ret = write(sockfd, buf, strlen(buf));
			if (strncasecmp(buf, "Done", strlen(buf)) == 0) {
				close(sockfd);
			}
		}
	}
}

