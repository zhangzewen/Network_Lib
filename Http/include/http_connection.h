#ifndef _HTTP_CONNECTION_H_INCLUDED__
#define _HTTP_CONNECTION_H_INCLUDED__

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "str.h"

#include "http_request.h"
#include "http_buffer.h"
#include "net.h"

typedef struct http_connection_st http_connection_t;

struct http_connection_st {
	struct event *read;
	struct event *write;
	listen_t *ls;
	http_request_t *r;
	struct http_buffer *buf;
	int fd;
	unsigned int requests; //统计这个连接上的http请求数，在删除连接的时候判断http请求数是否为0的时候做下一步判断
};

void accept_connection(struct event *rev);
void http_connection_close(http_connection_t *c);
#endif
