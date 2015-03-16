#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>

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
		fcntl(listenfd, F_SETFL, NONBLOCK);
	}
	return listenfd;
}


int main(int argc, char** argv)
{
	int connfd = 0;
	int listenfd = 0;
	int epollfd;
	struct epoll_event ev;
	struct epoll_event events[1024];
	struct sockaddr_in cliaddr; // cli addr
	socklen_t len = sizeof(cliaddr);
	listenfd = createSocketAndListen();
	epollfd = epoll_create(10);
	if (epollfd < 0) {
		fprintf(stderr, "Create epollfd Error!\n");
		exit (-1);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
		fprintf(stderr, "epoll_ctl add listenfd Error!\n");
		exit(-1);
	}

	while(1) {
		int nfds = epoll_wait(epollfd, events, 1024, -1);
		if (nfds == -1) {
			fprintf(stderr, "epoll_wait error!");
			exit(-1);
		}
		for (int i = 0; i < nfds; ++i) {
			if (event[i].data.fd == listenfd) {
				connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
				if (connfd < 0) {
					continue;
				}
				fcntl(connfd, F_SETFL, NONBLOCK);
				ev.events = EPOLLIN;
				ev.data.fd = connfd;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1) {
					fprintf(stderr, "epoll_ctl add connfd error!");
				}
				char buf[1024] = {0};
				int ret = read(connfd, buf, 1024);
				fprintf(stderr, "read: %s\n", buf);
				ret = write(connfd, buf, strlen(buf));
				close(connfd);
			}
		}
	}
	return 0;
}
