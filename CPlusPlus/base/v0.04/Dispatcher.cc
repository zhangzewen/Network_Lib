#include "Dispatcher.h"

Dispatcher::Dispatcher()
{
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
Dispatcher::poll()
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
			channel->handleEvent();
		}
	}
}

bool Dispatcher::addEvent(Channel* channel, int event)
{
	int revents = channel->getEvents();
	int fd = channel->getFd();
	int op = 0; // EPOLL_CTL_ADD ,default


	if (channels_.find(fd) != channels_.end()) {
		op = EPOLL_CTL_MOD;
	} else {
		op = EPOLL_CTL_ADD;
		channels_[fd] = channel;
	}
	revents |= event;
	channel->setEvent(revents);

	struct epoll_event ev;
	ev.events = events;
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

	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;

	if (channels_.find(fd) != channels_.end()) {
		op = EPOLL_CTL_MOD;
	} else {
		// shoule raise this fd does not register
	}
		if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
			std::cerr << "epoll_ctl add connfd error!" << std::endl;
			return false;
		}
	return true;
}

Channel* Dispatcher::findChannel(int fd, int events)
{
	if (!channels_.size()) {
		return NULL;
	}

	if (channels_.find(fd) != channels_.end()) {
		return channels_[fd];
	}
	return NULL;
}

