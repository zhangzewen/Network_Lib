#include "reactor.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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
    int createSocketAndListen(bool nonBlocking)
    {
        struct sockaddr_in srvaddr;
        int listenfd;
        socklen_t len = sizeof(srvaddr);
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
        if (listenfd < 0) {
            fprintf(stderr, "socket error!\n");
            exit(-1);
        }
        srvaddr.sin_family = AF_INET;
        srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        srvaddr.sin_port = htons(8080);
        len = sizeof(srvaddr);
        if (bind(listenfd, (struct sockaddr*)&srvaddr, len) < 0) {
            fprintf(stderr, "socket bind error!\n");
            exit(-1);
        }
        listen(listenfd, 100);
        if (listenfd < 0) {
            fprintf(stderr, "listen error\n");
            exit(-1);
        }
        if (nonBlocking) {
            fcntl(listenfd, F_SETFL, O_NONBLOCK);
        }
        return listenfd;
    }

    int SetNoblock(int fd)
    {
        int flags;

        if((flags = fcntl(fd, F_GETFL)) == -1) {
            return -1;
        }

        if((fcntl(fd, F_SETFL, flags | O_NONBLOCK)) == -1) {
            return -1;
        }

        return 0;
    }

    void ServerListening(std::shared_ptr<Event> ev)
    {
        int cfd;
        std::shared_ptr<Dispatcher> disp = ev->getDispatcher();
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);

        cfd = accept(ev->getFd() ,(struct sockaddr *)&addr, &addrlen);

        if(cfd == -1) {
            fprintf(stderr, "accept(): can not accept client connection");
            return;
        }

        if(SetNoblock(cfd) == -1) {
            close(cfd);
            return;
        }
        write(cfd, return_ok, strlen(return_ok));
        close(cfd);
        disp->delReadEvent(ev);
    }
};

int main()
{
	int listen_fd;
	//int i = 0;
	//int err = 0;
	Echo echoServer;

    std::shared_ptr<Dispatcher> dis(new Dispatcher());

	listen_fd = echoServer.createSocketAndListen(true);
	if(listen_fd < 0) {
		fprintf(stderr, "create socket error!");
		exit(1);
	}
    std::shared_ptr<Epoll> poller (new Epoll());
    poller->init();
    dis->setPoller(poller);
    dis->addReadEvent(listen_fd, std::bind(&Echo::ServerListening, &echoServer, _1));
    dis->loop();
	return 0;
}


