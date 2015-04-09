#ifndef _ACCEPTOR_H_INCLUDED__
#define _ACCEPTOR_H_INCLUDED__

#include "EventCallBack.h"

class Acceptor : public EventCallBack
{
public:
	Acceptor(int epollfd) : epollfd_(epollfd) {}
	~Acceptor();
	int createSocketAndListen(bool nonblocking);
	void setEvents(int event);
	//int registerEvent();
	int getSockfd()const;
	int setNonBlock(int fd);
	int start();
	void readEventHandle();
	void writeEventHandle();
	void timeOutEventHandle();
	void errorEventHandle();
private:
	int listenfd_;
	int epollfd_;
	int events_;
};
#endif
