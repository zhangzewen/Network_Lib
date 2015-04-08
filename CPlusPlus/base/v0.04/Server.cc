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

	acceptor_->setPollfd(epollfd_);
	if (acceptor_->start() < 0) {
		std::cerr << "Start server error!" << std::endl;
	}
	while(1) {
		int nfds = epoll_wait(epollfd_, events_, 1024, -1);
		if (nfds == -1) {
			std::cerr << "epoll_wait error!" << std::endl;
			exit(-1);
		}
		for (int i = 0; i < nfds; ++i) {
			std::shared_ptr<Channel> channel = std::make_shared<Channel>(static_cast<Channel*>(events_[i].data.ptr));
			channel->handleEvent();
		}
	}
}

