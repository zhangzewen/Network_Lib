#ifndef _CHANNEL_H_INCLUDED__
#define _CHANNEL_H_INCLUDED__

#include "ChannelCallBack.h"
class Channel
{
public:
	Channel(int epollfd, int sockfd);
	~Channel();
	void setCallBack(ChannelCallBack* callback);
	void handleEvent();
private:
	int epollfd_;
	int fd_;
	ChannelCallBack* callBack_;
};

#endif

