#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <assert.h>


#include "event.h"
#include "event_base.h"

static char return_ok[] = "HTTP/1.1 200 OK\r\nHost: 192.168.10.65\r\nConnection: close\r\n\r\nFinally, it works!";
	

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

void ServerRead(int fd, short events, void *arg)
{
	struct event *ev = (struct event *)arg;
	
	
	assert(events != 0);
	assert(arg != NULL);
			
	int nread = 0;
	char buff[1024] = {0};	
	nread = read(fd, buff, sizeof(buff) - 1);

	if (nread  == -1) {
		event_del(ev);
	}
	
	write(fd ,return_ok, sizeof(return_ok));
	close(fd);

	event_del(ev);
}

void ServerAccept(int fd, short events, void *arg)
{
	int cfd;
	struct sockaddr_in addr;
	struct event *cli_ev;
	socklen_t addrlen = sizeof(addr);
	cli_ev = calloc(1, sizeof(struct event));

	assert(events != 0);
	assert(arg != NULL);
	cfd = accept(fd ,(struct sockaddr *)&addr, &addrlen);
	
	if(cfd == -1) {
		printf("accept(): can not accept client connection");
		return;
	}

	if(SetNoblock(cfd) == -1) {
		close(cfd);
		return;
	}

	event_set(cli_ev, cfd, EV_READ | EV_PERSIST, ServerRead, (void *)cli_ev, NULL);
	event_add(cli_ev, NULL);
}


int main(int argc, char *argv[])
{
	int listen_fd;
	
	event_init();
	struct event ev;
	
	struct sockaddr_in server_addr;
	socklen_t len;
	if(3 != argc) {
		fprintf(stderr, "Usage: %s <server_ip> <server port>", argv[0]);
		exit(1);
	}

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);	
	if(listen_fd < 0) {
		fprintf(stderr, "create socket error!");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
	len = sizeof(server_addr);

	if(bind(listen_fd, (struct sockaddr *)&server_addr, len) < 0) {
		fprintf(stderr, "bind error!");
		exit(1);
	}
	
	

	if(listen(listen_fd, 1024) < 0){
		fprintf(stderr, "listen error!\n");
		exit(1);
	}

	int flags = fcntl(listen_fd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(listen_fd, F_SETFL, flags);

	event_set(&ev, listen_fd ,EV_READ | EV_PERSIST, ServerAccept, (void *)&ev, NULL);

	event_add(&ev, NULL);

	event_dispatch();
	
	return 0;
	
}
