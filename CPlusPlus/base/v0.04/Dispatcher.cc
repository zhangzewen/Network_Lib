#include "Dispatcher.h"

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
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
