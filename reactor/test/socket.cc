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
	int ret = listen(listenfd, 100);
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

void ServerRead(Event *ev)
{
	int nread = 0;
	char buff[1024] = {0};
	nread = read(ev->fd_, buff, sizeof(buff) - 1);

	write(ev->getFd() ,return_ok, sizeof(return_ok));
	close(ev->getFd());
}

void ServerAccept(Event* ev)
{
}
void ServerListening(Event* ev)
{
	int cfd;
	struct sockaddr_in addr;
	struct event *cli_ev;
	socklen_t addrlen = sizeof(addr);

	cli_ev = calloc(1, sizeof(struct event));

	cfd = accept(ev->fd_ ,(struct sockaddr *)&addr, &addrlen);

	if(cfd == -1) {
		fprintf(stderr, "accept(): can not accept client connection");
		return;
	}

	if(SetNoblock(cfd) == -1) {
		close(cfd);
		return;
	}
}

int main(int argc, char *argv[])
{
	int listen_fd;
	int i = 0;
	int err = 0;

    Dispatcher* dis = new Dispatcher();

	listen_fd = createSocketAndListen(true)
	if(listen_fd < 0) {
		fprintf(stderr, "create socket error!");
		exit(1);
	}

	return 0;
}

