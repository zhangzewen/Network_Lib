#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_

#include <stdio.h>
#include <sys/epoll.h>
#include "ChannelCallBack.h"
#include "AcceptorCallBack.h"
#include "Acceptor.h"
#include "Dispatcher.h"

class TcpServer
{
public:
	TcpServer() : epollfd_(-1), acceptor_(NULL), base_(NULL) {
	}
	~TcpServer();
	void Run();
private:
	int epollfd_;
	Acceptor* acceptor_;
	Dispatcher* base_;
};

#endif
