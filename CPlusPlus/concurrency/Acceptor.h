#ifndef _ACCEPTOR_H_INCLUDED__
#define _ACCEPTOR_H_INCLUDED__

#include "EventCallBack.h"
#include "Dispatcher.h"
#include <memory>

class Dispatcher;

class Acceptor : public EventCallBack, public std::enable_shared_from_this<Acceptor>
{
public:
	Acceptor(std::weak_ptr<Dispatcher> base);
	Acceptor();
	virtual ~Acceptor();
	int createSocketAndListen(bool nonblocking);
	void setEvents(int event);
	int getSockfd()const;
	int setNonBlock(int fd);
	int start();
	void setDispatcher(std::weak_ptr<Dispatcher> base);
	std::weak_ptr<Dispatcher> getDispatcher() const;
	void readEventHandle();
	void writeEventHandle();
	void timeOutEventHandle();
	void errorEventHandle();
private:
	int listenfd_;
	int epollfd_;
	int events_;
	std::weak_ptr<Dispatcher> base_;
};
#endif
