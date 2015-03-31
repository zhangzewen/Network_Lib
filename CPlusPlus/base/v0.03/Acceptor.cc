#include "Acceptor.h"
#include "Channel.h"
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <iostream>

int Acceptor::setNonBlock(int fd)
{
	int ret;
	if ((ret = fcntl(fd, F_GETFL)) < 0) {
		std::cerr << "fnctl F_GETFL error!" << std::endl;
		return -1;
	}
	ret |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, ret) < 0) {
		std::cerr << "fcntl F_SETFL NONBLOCK errot!" << std::endl;
		return -1;
	}
	return 0;
}

int Acceptor::createSocketAndListen(bool nonblocking)
{
	int listenfd_;
	socklen_t len;
	struct sockaddr_in srvaddr;
	listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd_ < 0) {
		std::cerr << "socket error!" << std::endl;
		return -1;
	}
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(8080);
	len = sizeof(srvaddr);
	if (bind(listenfd_, (struct sockaddr*)&srvaddr, len) < 0) {
		std::cerr << "socket bind error!" << std::endl;
		return -1;
	}
	int ret = listen(listenfd_, 100);
	if (listenfd_ < 0) {
		std::cerr << "listen error" << std::cout;
		return -1;
	}
	if (nonblocking) {
		if (setNonBlock(listenfd_) < 0) {
			std::cerr << "listenfd set nonblocking error!" << std::endl;
			return -1;
		}
	}
	return 0;
}


void Acceptor::setEvents(int event)
{
	events_ = event;
}

int Acceptor::registerEvent()
{
	struct epoll_event ev;
	ev.data.fd = listenfd_;
	ev.data.ptr = static_cast<void*>(this);
	ev.events = events_;
	if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, listenfd_, &ev) < 0) {
		std::cerr << "register listen event error!" << std::endl;
		return -1;
	}
	return 0;
}

int Acceptor::getSockfd()const
{
	return listenfd_;
}

void Acceptor::callBack(int sockfd)
{
	int connfd = -1;
	struct sockaddr_in cliaddr; // cli addr
	struct epoll_event ev;
	socklen_t len = sizeof(cliaddr);
	if (sockfd == listenfd_) {
		connfd = accept(listenfd_, (struct sockaddr*)&cliaddr, &len);
		if (connfd < 0) {
			return;
		}
		if (setNonBlock(connfd) < 0) {
			std::cerr << "connection set nonblocking error!" << std::endl;
			return;
		}

		Channel* channel = new Channel(epollfd_, connfd);
		channel->setEvents(EPOLLIN);
		channel->setCallBack(this);
		if (NULL == channel) {
			std::cerr << "Create Channel error!" << std::endl;
			return ;
		}
		if (channel->registerEvent() != 0) {
			std::cerr << "RegisterEvent error!" << std::endl;
			return ;
		}
	}
}

int Acceptor::start()
{
	if (createSocketAndListen(true) < 0) {
		std::cout << "CreateSocketAndListen Error!" << std::endl;
		return -1;
	}
	Channel* channel = new Channel(epollfd_, listenfd_);
	channel->setCallBack(this);
	channel->setEvents(EPOLLIN);
	if (channel->registerEvent() != 0) {
		std::cerr << "listen Event register Error!" << std::endl;
		return -1;
	}
}
