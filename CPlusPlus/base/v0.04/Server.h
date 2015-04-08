#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_

#include <stdio.h>
#include <sys/epoll.h>
#include <memory>
#include <iostream>
#include "ChannelCallBack.h"
#include "AcceptorCallBack.h"
#include "Acceptor.h"
#include <map>

class TcpServer : public AcceptorCallBack
{
public:
	TcpServer() : epollfd_(-1), acceptor_(new Acceptor()) {
	}
	~TcpServer();
	void Run();
private:
	int epollfd_;
	std::shared_ptr<Acceptor> acceptor_;
	struct epoll_event events_[1024];
	std::map<int fd, std::shared_ptr<Channel> > registedChannels_;
};

#endif
