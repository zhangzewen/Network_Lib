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

#include "http_epoll.h"
#include "event.h"
#include "http_parser.h"
#include "http_request.h"
#include "http_connection.h"
#include "str.h"

#if 0
void visit(void *data)
{
	struct header_keyval *v = (struct header_keyval *)data;
	fprintf(stderr, "[%s]: %s\n", v->key->data, v->value->data);
}


int
dontcall_message_begin_cb (http_parser *p)
{
	if (p) {
		
	}
  fprintf(stderr, "\n\n*** on_message_begin() called on paused parser ***\n\n");
	return 0;
}

int
dontcall_header_field_cb (http_parser *p, const char *buf, size_t len)
{
	assert( p != NULL);
	assert( buf != NULL);
	assert( len != 0);
	http_request_t *r = (http_request_t *)p->data;
  fprintf(stderr, "\n\n*** on_header_field() called on paused parser ***\n\n");

	r->kv = (struct header_keyval *)calloc(1, sizeof(struct header_keyval));
	if (!r->kv) {
		return -1;
	}
		
	r->kv->key = str_dup(buf, len);

	if (!r->kv->key) {
		return -1;
	}
	
	return 0;
}

int
dontcall_header_value_cb (http_parser *p, const char *buf, size_t len)
{
	assert( p != NULL);
	assert( buf != NULL);
	assert( len != 0);
	http_request_t *r = (http_request_t *)p->data;
  fprintf(stderr, "\n\n*** on_header_field() called on paused parser ***\n\n");
	if (!r->kv) {
		return -1;
	}	
	r->kv->value = str_dup(buf, len);

	if (!r->kv->value) {
		return -1;
	}

	add_record(r->http_request_headers, (void *)r->kv->key->data, (void *)r->kv);
	
	r->kv = NULL;
	
	return 0;
}

int
dontcall_request_url_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_request_url() called on paused parser ***\n\n");
	char buffer[1024] = {0};
	sprintf(buffer, buf, len);
	fprintf(stderr, "[%d]:%s\n", len, buffer);
	return 0;
}

int
dontcall_body_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_body_cb() called on paused parser ***\n\n");
	return 0;
}

int
dontcall_headers_complete_cb (http_parser *p)
{
  if (p) { } // gcc
  fprintf(stderr, "\n\n*** on_headers_complete() called on paused "
                  "parser ***\n\n");
	return 0;
}

int
dontcall_message_complete_cb (http_parser *p)
{
  fprintf(stderr, "\n\n*** on_message_complete() called on paused "
                  "parser ***\n\n");
	http_request_t *r = (http_request_t *)p->data;
	hashtable_dump(r->http_request_headers, visit);
	
	return 0;
}

int
dontcall_response_status_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_status() called on paused parser ***\n\n");
	char buffer[1024] = {0};
	sprintf(buffer, buf, len);
	fprintf(stderr, "[%d]:%s\n", len, buffer);
	return 0;
}


static char return_ok[] = "HTTP/1.1 200 OK\r\nHost: 192.168.10.65\r\nConnection: close\r\n\r\n尼玛，终于让老子给你跑通了啊！混蛋！";
	

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
//这里面的逻辑和nginx类似，应为这个水平触发，所以不需要重新添加事件（边缘触发需要添加read事件），如有数据会一直触发
//因该封装一个解析函数，
void ServerRead(int fd, short events, void *arg)
{
	struct event *ev = (struct event *)arg;
	http_request_t *r = NULL;
	r = http_request_create();
	if (NULL == r) {
		close(fd);
		event_del(ev);
	}
	http_parser_settings settings = {.on_message_begin = dontcall_message_begin_cb
  ,.on_header_field = dontcall_header_field_cb
  ,.on_header_value = dontcall_header_value_cb
  ,.on_url = dontcall_request_url_cb
  ,.on_status = dontcall_response_status_cb
  ,.on_body = dontcall_body_cb
  ,.on_headers_complete = dontcall_headers_complete_cb
  ,.on_message_complete = dontcall_message_complete_cb
  };

	http_parser *parser = malloc(sizeof(http_parser));
	http_parser_init(parser, HTTP_REQUEST);
	parser->data = (void *)r;
	size_t nparsed = 0;
	char buff[4096] = {0};	
	
	size_t nread = 0;
	
	nread = read(fd, buff, sizeof(buff) - 1);
	printf("\n----------------------------------------\n");
	fprintf(stderr, "nread = %d\n", nread);
	printf("\n----------------------------------------\n");
	nparsed = http_parser_execute(parser, &settings, buff, nread);
	if (parser->upgrade) {
		//do something
	} else if (nparsed != nread) {
		//Handle error. Usually just close the connection.
	}
#if 0
	write(fd ,return_ok, sizeof(return_ok));
	close(fd);

	event_del(ev);
#endif
}

void ServerAccept(int fd, short events, void *arg)
{
	int cfd;
	struct sockaddr_in addr;
	struct event *cli_ev;
	socklen_t addrlen = sizeof(addr);
	cli_ev = calloc(1, sizeof(struct event));

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
#endif


int main(int argc, char *argv[])
{
	struct event_base *base;
	base = event_base_new();
	struct event ev;
	http_listen_t *ls;
	
	if(4 != argc) {
		fprintf(stderr, "Usage: %s <server_ip> <server port> <backlog>", argv[0]);
		exit(1);
	}

 	ls = tcp_listen(argv[1], atoi(argv[2]), atoi(argv[3]));

 	start_listen(ls); 

	event_set(base, &ev, ls->fd ,EV_READ | EV_PERSIST, accept_connection, (void *)ls, NULL);

	event_add(&ev, NULL);

	event_base_dispatch(base);
	
	return 0;
	
}
