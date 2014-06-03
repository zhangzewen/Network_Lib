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
#include "Thread.h"

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

void ServerRead(struct event *ev)
{
	int nread = 0;
	char buff[1024] = {0};	
	nread = read(ev->ev_fd, buff, sizeof(buff) - 1);

	if (nread  == -1) {
		event_del(ev);
	}
	write(ev->ev_fd ,return_ok, sizeof(return_ok));
	close(ev->ev_fd);
	event_del(ev);
}

int ServerAccept(int fd, short events, struct event_base *base, void *arg)
{
	thread_task_t *task = (thread_task_t *)arg;
	fprintf(stderr, "[thread:%d|task: %d]Hear!, \n", task->thread->thread_id, *((int *)task->arg));
	#if 0

	event_base_set(base, cli_ev, cfd, EV_READ | EV_PERSIST, , (void *)cli_ev, NULL);
	event_add(cli_ev, NULL);
#endif
	struct event *ev = NULL;
	ev = (struct event *)calloc(1, sizeof(struct event));
	event_set(base, ev, fd, events, ServerRead, (void *)ev, NULL);
	event_add(ev, NULL);
}

void ServerListening(struct event *ev)
{
	int cfd;
	struct sockaddr_in addr;
	struct event *cli_ev;
	thread_pool_t *pool = (thread_pool_t *)ev->ev_arg;
	socklen_t addrlen = sizeof(addr);
	cli_ev = calloc(1, sizeof(struct event));

	cfd = accept(ev->ev_fd ,(struct sockaddr *)&addr, &addrlen);
	
	if(cfd == -1) {
		printf("accept(): can not accept client connection");
		return;
	}

	if(SetNoblock(cfd) == -1) {
		close(cfd);
		return;
	}

#if 0
	event_set(cli_ev, cfd, EV_READ | EV_PERSIST, , (void *)cli_ev, NULL);
	event_add(cli_ev, NULL);
#endif
	dispatch_conn_new(cfd, EV_READ | EV_PERSIST, (void *)pool);
}


int main(int argc, char *argv[])
{
	int listen_fd;
	int i = 0;
	
	struct event_base *new = event_base_new();
	struct event ev;
	thread_pool_t *pool = NULL;
	int num[3] = {0};
	
	struct sockaddr_in server_addr;
	socklen_t len;
	if(3 != argc) {
		fprintf(stderr, "Usage: %s <server_ip> <server port>", argv[0]);
		exit(1);
	}

	pool = thread_pool_create(3);

	for(i = 0; i < 3; ++i) {
		num[i] = i + 1;
		thread_pool_add_task(pool, ServerAccept, (void *)(&num[i]));
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

	event_set(new, &ev, listen_fd ,EV_READ | EV_PERSIST, ServerListening, (void *)pool, NULL);

	event_add(&ev, NULL);
	
	event_base_dispatch(new);
	event_base_free(&new);

	
	return 0;
	
}
