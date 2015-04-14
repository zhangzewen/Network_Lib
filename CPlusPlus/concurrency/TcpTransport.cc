#include "TcpTransport.h"
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

TcpTransport::TcpTransport(int epollfd, int sockfd)
{
	std::cout << "TcpTransprot::TcpTransprot()" << std::endl;
	epollfd_ = epollfd;
	connfd_ = sockfd;
}

TcpTransport::~TcpTransport()
{
	close(connfd_);
	std::cout << "TcpTransprot::~TcpTransprot()" << std::endl;
}

void TcpTransport::readEventHandle()
{
		char buf[1024] = {0};
		int ret = read(connfd_, buf, 1024);
		std::cerr << "read: " << buf << std::endl;
		ret = write(connfd_, buf, strlen(buf));
		if (strncasecmp(buf, "Done", strlen(buf)) == 0) {
			close(connfd_);
		}
}
void TcpTransport::writeEventHandle()
{
}

void TcpTransport::timeOutEventHandle()
{
}

void TcpTransport::errorEventHandle()
{
}

void TcpTransport::setEvents(int event)
{
	events_ = event;
}

int TcpTransport::getSockfd()const
{
	return connfd_;
}
