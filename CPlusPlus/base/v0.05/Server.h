#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_

#include <stdio.h>
#include <sys/epoll.h>
#include <memory>
#include "Acceptor.h"
#include "Dispatcher.h"

class TcpServer
{
public:
	TcpServer() : epollfd_(-1), acceptor_(new Acceptor()), base_(new Dispatcher()) {
	}
	~TcpServer();
	void Run();
private:
	int epollfd_;
	std::shared_ptr<Acceptor> acceptor_;
	std::shared_ptr<Dispatcher> base_;
};

#endif
