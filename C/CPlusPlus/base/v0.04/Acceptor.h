#ifndef _ACCEPTOR_H_INCLUDED__
#define _ACCEPTOR_H_INCLUDED__

#include "EventCallBack.h"
#include "Dispatcher.h"

class Acceptor : public EventCallBack
{
public:
	Acceptor(Dispatcher* base);
	Acceptor();
	~Acceptor();
	int createSocketAndListen(bool nonblocking);
	void setEvents(int event);
	int getSockfd()const;
	int setNonBlock(int fd);
	int start();
	void setDispatcher(Dispatcher* base);
	Dispatcher* getDispatcher() const;
	void readEventHandle();
	void writeEventHandle();
	void timeOutEventHandle();
	void errorEventHandle();
private:
	int listenfd_;
	int epollfd_;
	int events_;
	Dispatcher* base_;
};
#endif
