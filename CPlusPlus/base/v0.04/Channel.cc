#include "Channel.h"
#include <sys/epoll.h>
#include <iostream>

Channel::Channel(int sockfd, Dispatcher* base)
{
	fd_ = sockfd;
	base_ = base;
}

Channel::Channel(int sockfd) : base_(NULL)
{
	fd_ = sockfd;
}

Channel::Channel() : fd_(-1), base_(NULL)
{
}

Channel::~Channel()
{
}

void Channel::setFd(int fd)
{
	fd_ = fd;
}

void Channel::setDispatcher(Dispatcher* base)
{
	base_ = base;
}

void Channel::setCallBack(EventCallBack* callback)
{
	callBack_ = callback;
}

void Channel::handleEvent(int activeEvents)
{
	if (activeEvents & EPOLLIN) { //read event
		callBack_->readEventHandle();
	} else if (activeEvents & EPOLLOUT) { //write event
		callBack_->writeEventHandle();
	} else if (activeEvents & 0x20){ // time out event ,there will be a timeout macro
		callBack_->timeOutEventHandle();
	} else { // others are error event
		callBack_->errorEventHandle();
	}
}

int Channel::getFd() const
{
	return fd_;
}

int Channel::setEvents(int event)
{
	events_ = event;
}

int Channel::registerEvent(int events)
{
	base_->addEvent(this, events);
	return 0;
}


int Channel::unRegisterEvent(int events)
{
	base_->delEvent(this, events);
	return 0;
}


int Channel::getEvents()const
{
	return events_;
}
