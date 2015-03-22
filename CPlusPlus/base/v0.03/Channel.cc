#include "Channel.h"

Channel::Channel(int epollfd, int sockfd)
{
	epollfd_ = epollfd;
	fd_ = sockfd;
}

Channel::~Channel()
{
}

void Channel::setCallBack(ChannelCallBack* callback)
{
	callBack_ = callback;
}

void Channel::handleEvent()
{
	callBack_->callBack(fd_);
}

