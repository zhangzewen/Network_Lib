#include "TcpTransport.h"
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <functional>

TcpTransport::TcpTransport(int epollfd, int sockfd)
{
	epollfd_ = epollfd;
	connfd_ = sockfd;
}

TcpTransport::~TcpTransport()
{
	close(connfd_);
}

void TcpTransport::callBack()
{
		char buf[1024] = {0};
		int ret = read(connfd_, buf, 1024);
		std::cerr << "read: " << buf << std::endl;
		ret = write(connfd_, buf, strlen(buf));
		if (strncasecmp(buf, "Done", strlen(buf)) == 0) {
			close(connfd_);
		}
}

int TcpTransport::registerEvent()
{
	struct epoll_event ev;
	ev.events = events_;
	ev.data.fd = connfd_;
	ev.data.ptr = static_cast<void*>(this);
	if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, connfd_, &ev) < 0) {
		std::cerr << "connection register error!" << std::endl;
		return -1;
	}
	return 0;
}

void TcpTransport::setEvents(int event)
{
	events_ = event;
}

int TcpTransport::getSockfd()const
{
	return connfd_;
}
