#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>

#include "http_connection.h"
#include "http_buffer.h"

#if 0
http_listen_t *tcp_listen(const char *addr, const int port, const int backlog)
{
	http_listen_t *new = NULL;
	
	new = (http_listen_t *)calloc(1, sizeof(struct http_listen_st));
	if (!new) {
		return NULL;
	}

	new->fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(new->fd < 0) {
		fprintf(stderr, "create socket error!");
		free(new);
		return NULL;
	}

	new->sockaddr.sin_family = AF_INET;
	new->sockaddr.sin_port = htons(port);
	int s = inet_pton(AF_INET, addr, &new->sockaddr.sin_addr);
	if (s <= 0) {
		free(new);
		return NULL;
	}

	new->socklen = sizeof(new->sockaddr);
	new->addr_text = str_dup(addr, strlen(addr));
	if (!new->addr_text) {
		free(new);
		return NULL;
	}
	
	new->backlog = backlog;
	new->listen = 0;
	new->nonblocking = 0;

	return new;
}


int start_listen(http_listen_t *ls)
{
	
	if (!ls) {
		return -1;
	}
	int flags = fcntl(ls->fd, F_GETFL);
  flags |= O_NONBLOCK;
  fcntl(ls->fd, F_SETFL, flags);
	ls->nonblocking = 1;
	if(bind(ls->fd, (struct sockaddr *)&ls->sockaddr, ls->socklen) < 0) {
		fprintf(stderr, "bind error!");
		return -1;	
	}

	if(listen(ls->fd, ls->backlog) < 0){
		fprintf(stderr, "listen error!\n");
		return -1;
	}

	ls->listen = 1;
	return 0;
}
#endif
 
void connection_free(http_connection_t *c)
{
}
http_connection_t *get_connection(void *data)
{
	
}


void accept_connection(struct event *rev)
{
	http_connection_t *c;
	listen_t *ls = (listen_t *)(rev->ev_arg);
	struct sockaddr_in sockaddr;
	socklen_t len;
	int cfd = -1;
	
	cfd = accept(ls->fd, (struct sockaddr *)&sockaddr, &len);	
	if (cfd < 0) {
		if (errno =EAGAIN) {
			fprintf(stderr, "accept() not ready!\n");
			return ;
		}
	}

	c = (http_connection_t *)calloc(1, sizeof(http_connection_t));
	if (!c) {
		close(cfd);
		return ;
	}

	c->read = (struct event *)calloc(1, sizeof(struct event));
	if (!c->read) {
		close(cfd);
		free(c);
		return ;
	}

	c->write = (struct event *)calloc(1, sizeof(struct event));
	if (!c->write) {
		close(cfd);
		free(c->read);
		free(c);
		return ;
	}

	int flags = fcntl(cfd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(cfd, F_SETFL, flags);

	c->fd = cfd;
	c->ls = ls;
	c->buf = buffer_new();
	connection_init(c);
}



int connection_init(http_connection_t *c)
{
	struct event *rev;
	struct event *snd;
	
	struct timeval tv;
	tv.tv_sec = 60;
	tv.tv_usec = 0;

	rev = c->read;
	snd = c->write;
	event_set(rev, c->fd, EV_READ | EV_PERSIST, http_init_request, (void*)c, NULL);
	event_add(rev, &tv);
}

void http_connection_close(http_connection_t *c)
{

	if (!c) {
		fprintf(stderr, "the connection is closed!\n");
	}

	event_del(c->read);
	event_del(c->write);
	close(c->fd);
	free(c->read);
	free(c->write);
	free(c);
}



