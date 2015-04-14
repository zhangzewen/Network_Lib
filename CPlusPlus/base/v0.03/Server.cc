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
	if ((epollfd_ = epoll_create(1)) < 0) {
		std::cerr << "Create epollfd error!" << std::endl;
		exit(-1);
	}
	acceptor_ = new Acceptor(epollfd_);
	if (NULL == acceptor_) {
		std::cerr << "Create Acceptor error!" << std::endl;
		exit(-1);
	}
	if (acceptor_->start() < 0) {
		std::cerr << "Start server error!" << std::endl;
	}
	int n = 10;
	while(n) {
		int nfds = epoll_wait(epollfd_, events_, 1024, -1);
		if (nfds == -1) {
			std::cerr << "epoll_wait error!" << std::endl;
			exit(-1);
		}
		for (int i = 0; i < nfds; ++i) {
			Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
			channel->handleEvent();
		}
		--n;
	}
}



#if 0
void TcpServer::acceptorCallBack() {
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
	return;
}
#endif

