#include "reactor.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>

#define return_ok "YES, it's work\r\n\r\n"

using namespace std::placeholders;

class Echo
{
public:
    void Print(std::shared_ptr<Event> ev) {
        assert(NULL != ev);
        std::cout<<"Print()" << std::endl;
    }

};

int main()
{
    std::shared_ptr<Dispatcher> dis(new Dispatcher());
    Echo echoServer;

    std::shared_ptr<Epoll> poller (new Epoll());
    poller->init();
    dis->setPoller(poller);
    dis->runAt(10, std::bind(&Echo::Print, &echoServer, _1));
    dis->loop();
	return 0;
}


