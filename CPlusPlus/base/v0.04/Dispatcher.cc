#include "Dispatcher.h"
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

Dispatcher::Dispatcher()
{
	channels_.clear();
}

Dispatcher::~Dispatcher()
{
}

bool Dispatcher::init()
{
	epollfd_ = epoll_create(1);
	if (epollfd_ < 0) {
		return false;
	}
	channels_.clear();
	return true;
}
void Dispatcher::poll()
{
	while(1) {
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
			Channel* channel = channels_[activefd];
			channel->handleEvent(events_[i].events);
		}
	}
}

bool Dispatcher::addEvent(Channel* channel, int event)
{
	int revents = channel->getEvents();
	int fd = channel->getFd();
	int op = 0; // EPOLL_CTL_ADD ,default

	Channel* temp = NULL;


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

bool Dispatcher::delEvent(Channel* channel, int events)
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

Channel* Dispatcher::findChannel(int fd)
{
	if (!channels_.size()) {
		return NULL;
	}

	if (channels_.find(fd) != channels_.end()) {
		return channels_[fd];
	}
	return NULL;
}

