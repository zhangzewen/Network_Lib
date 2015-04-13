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
	std::cout << "TcpServer::~TcpServer()" << std::endl;
}
void TcpServer::Run()
{

	//base_ = new EpollDispatcher();
	base_->init();

	//acceptor_ = new Acceptor();
	if (NULL == acceptor_) {
		std::cerr << "Create Acceptor error!" << std::endl;
		exit(-1);
	}
	acceptor_->setDispatcher(base_);
	if (acceptor_->start() < 0) {
		std::cerr << "Start server error!" << std::endl;
	}

	base_->poll();
}



