#include "Dispatcher.h"
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

EpollDispatcher::EpollDispatcher()
{
	std::cout << "EpollDispatcher::EpollDispatcher()" << std::endl;
	channels_.clear();
}

EpollDispatcher::~EpollDispatcher()
{
	std::cout << "EpollDispatcher::~EpollDispatcher()" << std::endl;
}

bool EpollDispatcher::init()
{
	epollfd_ = epoll_create(1);
	if (epollfd_ < 0) {
		return false;
	}
	channels_.clear();
	return true;
}
void EpollDispatcher::poll()
{
	while(n) {
		int nfds = epoll_wait(epollfd_, events_, 1024, -1);
		if (nfds == -1) {
			std::cerr << "epoll_wait error!" << std::endl;
			exit(-1);
		}
		for (int i = 0; i < nfds; ++i) {
			int activefd = events_[i].data.fd;
			if (channels_.find(activefd) == channels_.end()) {
				continue;
			}
			std::shared_ptr<Channel> channel = channels_[activefd];
			channel->handleEvent(events_[i].events);
		}
	}
}

bool EpollDispatcher::addEvent(std::shared_ptr<Channel> channel, int event)
{
	int revents = channel->getEvents();
	int fd = channel->getFd();
	int op = 0; // EPOLL_CTL_ADD ,default

	std::shared_ptr<Channel> temp = std::shared_ptr<Channel>();


	if ((temp = findChannel(fd)) != NULL) {
		op = EPOLL_CTL_MOD;
	} else {
		op = EPOLL_CTL_ADD;
		channels_[fd] = channel;
	}
	revents |= event;
	channel->setEvents(revents);

	struct epoll_event ev;
	ev.events = revents;
	ev.data.fd = fd;

	if (epoll_ctl(epollfd_, op, fd, &ev) == -1) {
		std::cerr << "epoll_ctl connfd error!" << std::endl;
		return false;
	}
	return true;
}

bool EpollDispatcher::delEvent(std::shared_ptr<Channel> channel, int events)
{
	int revents = channel->getEvents();
	int fd = channel->getFd();
	int op = 0;


	if (channels_.find(fd) != channels_.end()) {
		op = EPOLL_CTL_MOD;
	} else {
		// shoule raise this fd does not register
	}

	struct epoll_event ev;
	ev.events = revents;
	ev.data.fd = fd;
	
	if (epoll_ctl(epollfd_, op, fd, &ev) == -1) {
		std::cerr << "epoll_ctl add connfd error!" << std::endl;
		return false;
	}
	return true;
}

std::shared_ptr<Channel> EpollDispatcher::findChannel(int fd)
{
	if (!channels_.size()) {
		return NULL;
	}

	if (channels_.find(fd) != channels_.end()) {
		return channels_[fd];
	}
	return NULL;
}


//select
SelectDispatcher::SelectDispatcher()
{
}

SelectDispatcher::~SelectDispatcher()
{
}

bool SelectDispatcher::init()
{
}

void SelectDispatcher::poll()
{
}

bool SelectDispatcher::addEvent(std::shared_ptr<Channel> channel, int event)
{

}
bool SelectDispatcher::delEvent(std::shared_ptr<Channel> channel, int event)
{

}


//poll
PollDispatcher::PollDispatcher()
{
}

PollDispatcher::~PollDispatcher()
{
}

bool PollDispatcher::init()
{
}

void PollDispatcher::poll()
{
}

bool PollDispatcher::addEvent(std::shared_ptr<Channel> channel, int event)
{

}
bool PollDispatcher::delEvent(std::shared_ptr<Channel> channel, int event)
{

}
