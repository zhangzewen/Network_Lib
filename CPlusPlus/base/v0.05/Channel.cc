#include "Channel.h"
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>

Channel::Channel(int sockfd, std::weak_ptr<Dispatcher> base)
			: fd_(sockfd), base_(base), events_(0)
{
	std::cout << "Channel::Channel(int sockfd, std::shared_ptr<Dispatcher> base)" << std::endl;
}

Channel::Channel(int sockfd) : fd_(sockfd), base_(std::weak_ptr<Dispatcher>()), events_(0)
{
	std::cout << "Channel::Channel(int sockfd)" << std::endl;
}

Channel::Channel() : fd_(-1), base_(std::weak_ptr<Dispatcher>()), events_(0)
{
	std::cout << "Channel::Channel()" << std::endl;
}

Channel::~Channel()
{
	close(fd_);
	std::cout << "Channel::~Channel()" << std::endl;
}

void Channel::setFd(int fd)
{
	fd_ = fd;
}

void Channel::setDispatcher(std::weak_ptr<Dispatcher> base)
{
	base_ = base;
}

void Channel::setCallBack(std::shared_ptr<EventCallBack> callback)
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
	std::shared_ptr<Dispatcher> base = base_.lock();
	base->addEvent(shared_from_this(), events);
	return 0;
}


int Channel::unRegisterEvent(int events)
{
	std::shared_ptr<Dispatcher> base = base_.lock();
	base->delEvent(shared_from_this(), events);
	return 0;
}


int Channel::getEvents()const
{
	return events_;
}
