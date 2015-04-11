#include "Channel.h"
#include <sys/epoll.h>
#include <iostream>

Channel::Channel(int epollfd, int sockfd)
{
	epollfd_ = epollfd;
	fd_ = sockfd;
}

Channel::~Channel()
{
}

void Channel::setCallBack(std::shared_ptr<ChannelCallBack> callback)
{
	callBack_ = callback;
}

void Channel::handleEvent()
{
	callBack_->callBack(fd_);
}

int Channel::getSockfd() const
{
	return fd_;
}

int Channel::setEvents(int event)
{
	events_ |= event;
}

int Channel::clearEvents(int event)
{
	events_ &= ~event;
}

int Channel::registerEvent()
{
	struct epoll_event ev;
	ev.events = events_;
	ev.data.fd = fd_;
	ev.data.ptr = static_cast<void*>(shared_from_this().get());
	if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd_, &ev) == -1) {
		std::cerr << "epoll_ctl add connfd error!" << std::endl;
		return -1;
	}
	return 0;
}
