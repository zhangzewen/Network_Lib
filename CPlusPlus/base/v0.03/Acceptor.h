#ifndef _ACCEPTOR_H_INCLUDED__
#define _ACCEPTOR_H_INCLUDED__

#include "ChannelCallBack.h"

class Acceptor : public ChannelCallBack
{
public:
	Acceptor(int epollfd) : epollfd_(epollfd) {}
	~Acceptor() {}
	int createSocketAndListen(bool nonblocking);
	void setEvents(int event);
	int registerEvent();
	virtual void callBack(int fd);
	int getSockfd()const;
	int setNonBlock(int fd);
	int start();
private:
	int listenfd_;
	int epollfd_;
	int events_;
};
#endif
