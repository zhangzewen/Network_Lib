#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_

#include <stdio.h>
#include <sys/epoll.h>
#include "ChannelCallBack.h"
#include "AcceptorCallBack.h"
#include "Acceptor.h"

class TcpServer
{
public:
	TcpServer() : epollfd_(-1), acceptor_(NULL) {
	}
	~TcpServer();
	void Run();
private:
	int epollfd_;
	Acceptor* acceptor_;
	struct epoll_event events_[1024];
};

#endif
