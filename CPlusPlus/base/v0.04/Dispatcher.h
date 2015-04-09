#ifndef __DISPATCHER_H_INCLUDED__
#define __DISPATCHER_H_INCLUDED__

#include "Channel.h"
#include "ChannelCallBack.h"

class Dispatcher
{
public:
	Dispatcher(){}
	~Dispatcher(){}
	void poll();
private:
	int epollfd_;
	std::map<int fd, Channel*> channels_;

};

#endif //__DISPATCHER_H_INCLUDED__
