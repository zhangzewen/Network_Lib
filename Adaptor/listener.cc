#include "listener.h"
#include "util.h"
#include "connection.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <event.h>
#include <stdlib.h>
#include <iostream>

//listener::listener(struct event_base* base, const std::string& host,
//		int port) : base_(base), host_(host),
//			port_(port), listenfd_(-1), ev_(NULL) {
//
//}
listener::listener(struct event_base* base) : base_(base) {

}

void listener::makeConnection(int connfd, struct event_base* base)
{
    connection* conn = new connection(connfd, base);
    if (NULL == conn) {
        std::cout << "create connection error!" << std::endl;
    }
    conn->setListener(this);
    if (!conn->init()) {
        std::cout << "connection init error!" << std::endl;
    }
}


void listener::listenCallBack(int fd, short event, void* arg)
{
    listener* listen = static_cast<listener*>(arg);
    if (event & EV_READ) {
        struct event* ev = static_cast<struct event*> (arg);
        struct sockaddr_in cliaddr;
        socklen_t len;
        int connfd = 0;
        if ((connfd = accept(fd, (struct sockaddr*)&cliaddr, &len)) < 0) {
            std::cerr << "Make connection error!" << std::endl;
            return;
        }

        if (Util::setNonBlock(connfd) < 0) { // can not set nonblocking just return and lost this connection!
            return;
        }
        listen->makeConnection(connfd, listen->getEventBase());
    } else {
        std::cerr << "We do not care listener write or error!" << std::endl;
    }
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
	srvaddr.sin_port = htons(8080);
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

void listener::start()
{
    if (createSocketAndListen() < 0) {
        return ;
    }
    ev_  = (struct event*)malloc(sizeof(struct event));
    event_set(ev_, listenfd_, EV_READ |EV_PERSIST, listenCallBack, this);
    event_base_set(base_, ev_);
    event_add(ev_, NULL);
}

