#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_

#include <sys/epoll.h>
#include "ChannelCallBack.h"

class TcpServer : public ChannelCallBack
{
public:
	TcpServer() : listenfd_(-1), epollfd_(-1) {
	}
	~TcpServer();
	void Run();
	void callBack(int fd);
private:
	int createSocketAndListen(bool nonblocking);
	int setNonblock(int fd);
	int listenfd_;
	int epollfd_;
	struct epoll_event events_[1024];
};

#endif
