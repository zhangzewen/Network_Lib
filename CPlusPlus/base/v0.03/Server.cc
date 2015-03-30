#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/fcntl.h>
#include "Server.h"
#include "Channel.h"
#include "Acceptor.h"
#include <iostream>

TcpServer::~TcpServer()
{
	close(epollfd_);
}
void TcpServer::Run()
{
	int connfd = 0;
	struct epoll_event ev;
	acceptor_ = new Acceptor();

	if (acceptor_->createSocketAndListen(true) < 0) {
		std::cout << "CreateSocketAndListen Error!" << std::endl;
		return ;
	}
	epollfd_ = epoll_create(10);
	if (epollfd_ < 0) {
		std::cerr << "Create epollfd Error!" << std::endl;
		exit (-1);
	}

	Channel* channel = new Channel(epollfd_, listenfd_);
	channel->setCallBack(this);
	channel->setEvents(EPOLLIN);
	if (channel->registerEvent() != 0) {
		std::cerr << "listen Event register Error!" << std::endl;
		return;
	}

	while(1) {
		int nfds = epoll_wait(epollfd_, events_, 1024, -1);
		if (nfds == -1) {
			std::cerr << "epoll_wait error!" << std::endl;
			exit(-1);
		}
		for (int i = 0; i < nfds; ++i) {
			Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
			channel->handleEvent();
		}
	}
}


int TcpServer::setNonblock(int fd)
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
	} else {
		if (sockfd & EPOLLIN) {
			char buf[1024] = {0};
			int ret = read(sockfd, buf, 1024);
			std::cerr << "read: " << buf << std::endl;
			ret = write(sockfd, buf, strlen(buf));
			if (strncasecmp(buf, "Done", strlen(buf)) == 0) {
				close(sockfd);
			}
		}
	}
}

