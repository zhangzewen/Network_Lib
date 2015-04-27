#include "listener.h"
#include "util.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <event.h>
#include <iostream>

listener::listener(struct event_base* base, const std::string& host,
		int port) : base_(base), host_(host),
			port_(port), listenfd_(-1), ev_(NULL) {

}

int listener::createSocketAndListen()
{
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd_ < 0) {
        std::cerr <<  "socket error!" << std::endl;
		return -1;
	}
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(port_);
	len = sizeof(srvaddr);
	if (bind(listenfd_, (struct sockaddr*)&srvaddr, len) < 0) {
        std::cerr << "socket bind error!" << std::endl;
		return -1;
	}
	if (Util::setNonBlock(listenfd_) < 0) {
		return -1;
	}
	int ret = listen(listenfd_, 100);
	if (ret < 0) {
        std::cerr << "listen error" << std::endl;
		return -1;
	}
	if (Util::setNonBlock(listenfd_) < 0) {
		return -1;
	}
	return 0;
}

