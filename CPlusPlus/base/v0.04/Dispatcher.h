#ifndef __DISPATCHER_H_INCLUDED__
#define __DISPATCHER_H_INCLUDED__

#include <map>
#include <sys/epoll.h>
#include "ChannelCallBack.h"
#include "Channel.h"

class Channel;

class Dispatcher
{
public:
	Dispatcher(){}
	virtual ~Dispatcher(){}
	virtual bool init() = 0;
	virtual void poll() = 0;
	virtual bool addEvent(Channel*, int event) = 0;
	virtual bool delEvent(Channel*, int event) = 0;
};

class EpollDispatcher : public Dispatcher
{
public:
	EpollDispatcher();
	virtual ~EpollDispatcher();
	bool init();
	void poll();
	Channel* findChannel(int fd);
	bool addEvent(Channel*, int event);
	bool delEvent(Channel*, int event);
private:
	int epollfd_;
	std::map<int, Channel*> channels_;
	struct epoll_event events_[1024];
};


class SelectDispatcher : public Dispatcher
{
public:
	SelectDispatcher();
	virtual ~SelectDispatcher();
	bool init();
	void poll();
	bool addEvent(Channel*, int event);
	bool delEvent(Channel*, int event);
};

class PollDispatcher : public Dispatcher
{
public:
	PollDispatcher();
	virtual ~PollDispatcher();
	bool init();
	void poll();
	bool addEvent(Channel*, int event);
	bool delEvent(Channel*, int event);
};

#endif //__DISPATCHER_H_INCLUDED__
