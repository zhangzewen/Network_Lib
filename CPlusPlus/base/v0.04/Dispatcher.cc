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

bool Dispatcher::addEvent(Channel* channel)
{
	int events = channel->getEvents();
	int fd = channel->getFd();

	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;

	if (channels_.find(fd) != channels_.end()) { //means that that fd has already register,now just EPOLL_CTL_MOD
		if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
			std::cerr << "epoll_ctl add connfd error!" << std::endl;
			return false;
		}
	}

	channels_[fd] = channel;
	if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd_, &ev) == -1) {
		std::cerr << "epoll_ctl add connfd error!" << std::endl;
		return false;
	}
	return true;
}

bool Dispatcher::delEvent(Channel* channel, int events)
{

	//分del和mod两种模式
	int events = channel->getEvents();
	int fd = channel->getFd();

	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;

	if (channels_.find(fd) != channels_.end()) { //means that that fd has already register,now just EPOLL_CTL_MOD
		if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
			std::cerr << "epoll_ctl add connfd error!" << std::endl;
			return false;
		}
	}

	// channel is not exists, return true anyway!
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

