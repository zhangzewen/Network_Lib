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
	Dispatcher();
	~Dispatcher();
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

#endif //__DISPATCHER_H_INCLUDED__
