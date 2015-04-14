#ifndef __DISPATCHER_H_INCLUDED__
#define __DISPATCHER_H_INCLUDED__

#include <map>
#include <sys/epoll.h>
#include "Channel.h"
#include <memory>
#include <iostream>

class Channel;

class Dispatcher
{
public:
	Dispatcher() {}
	virtual ~Dispatcher() {std::cout << "Dispatcher::~Dispatcher()" << std::endl;}
	virtual bool init() {}
	virtual void poll() {}
	virtual bool addEvent(std::shared_ptr<Channel>, int) {}
	virtual bool delEvent(std::shared_ptr<Channel>, int) {}
};

class EpollDispatcher : public Dispatcher
{
public:
	EpollDispatcher();
	virtual ~EpollDispatcher();
	bool init();
	void poll();
	std::shared_ptr<Channel> findChannel(int fd);
	bool addEvent(std::shared_ptr<Channel>, int);
	bool delEvent(std::shared_ptr<Channel>, int);
private:
	int epollfd_;
	std::map<int, std::shared_ptr<Channel> > channels_;
	struct epoll_event events_[1024];
};


class SelectDispatcher : public Dispatcher
{
public:
	SelectDispatcher();
	virtual ~SelectDispatcher();
	bool init();
	void poll();
	bool addEvent(std::shared_ptr<Channel>, int);
	bool delEvent(std::shared_ptr<Channel>, int);
};

class PollDispatcher : public Dispatcher
{
public:
	PollDispatcher();
	virtual ~PollDispatcher();
	bool init();
	void poll();
	bool addEvent(std::shared_ptr<Channel>, int);
	bool delEvent(std::shared_ptr<Channel>, int);
};

#endif //__DISPATCHER_H_INCLUDED__
