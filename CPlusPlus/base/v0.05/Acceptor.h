#ifndef _ACCEPTOR_H_INCLUDED__
#define _ACCEPTOR_H_INCLUDED__

#include "ChannelCallBack.h"
#include "AcceptorCallBack.h"
#include <memory>
#include <iostream>

class Acceptor : public ChannelCallBack, public std::enable_shared_from_this<Acceptor>
{
public:
	Acceptor(int epollfd) : epollfd_(epollfd) {}
	Acceptor(){std::cout << "Acceptor() called" << std::endl;}
	~Acceptor();
	int createSocketAndListen(bool nonblocking);
	void setAcceptorCallBack(std::shared_ptr<AcceptorCallBack> callback);
	void setEvents(int event);
	int getSockfd()const;
	int setNonBlock(int fd);
	int setPollfd(int fd);
	int start();
	void callBack(int fd);
private:
	int listenfd_;
	int epollfd_;
	int events_;
	std::shared_ptr<AcceptorCallBack> acceptorCallBack;
};
#endif
