#include <event.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <http_parser.h>

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
    fprintf(stderr, "\n\n*** on_header_field() called on paused parser ***\n\n");
	return 0;
}

int
dontcall_header_value_cb (http_parser *p, const char *buf, size_t len)
{
    assert( p != NULL);
    assert( buf != NULL);
    assert( len != 0);
    fprintf(stderr, "\n\n*** on_header_field() called on paused parser ***\n\n");

	return 0;
}

int
dontcall_request_url_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_request_url() called on paused parser ***\n\n");
	char buffer[1024] = {0};
	sprintf(buffer, buf, len);
	return 0;
}

int
dontcall_body_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_body_cb() called on paused parser ***\n\n");
  fprintf(stderr, "%s", buf);
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
	return 0;
}


int createSocketAndListen()
{
	struct sockaddr_in srvaddr;
	int listenfd;
	socklen_t len = sizeof(srvaddr);
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
        std::cerr <<  "socket error!" << std::endl;
		exit(-1);
	}
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(8080);
	len = sizeof(srvaddr);
	if (bind(listenfd, (struct sockaddr*)&srvaddr, len) < 0) {
        std::cerr << "socket bind error!" << std::endl;
		exit(-1);
	}
	int ret = listen(listenfd, 100);
	if (ret < 0) {
        std::cerr << "listen error" << std::endl;
		exit(-1);
	}
	return listenfd;
}

int setNonBlock(int fd)
{
	int ret;
	if ((ret = fcntl(fd, F_GETFL)) < 0) {
		std::cerr << "fnctl F_GETFL error!" << std::endl;
		return -1;
	}
	ret |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, ret) < 0) {
		std::cerr << "fcntl F_SETFL NONBLOCK errot!" << std::endl;
		return -1;
	}
	return 0;
}

void onMessage(int fd, short event, void* arg)
{

    http_parser_settings settings;
    settings.on_message_begin = dontcall_message_begin_cb;
    settings.on_header_field = dontcall_header_field_cb;
    settings.on_header_value = dontcall_header_value_cb;
    settings.on_url = dontcall_request_url_cb;
    settings.on_body = dontcall_body_cb;
    settings.on_headers_complete = dontcall_headers_complete_cb;
    settings.on_message_complete = dontcall_message_complete_cb;

	http_parser *parser = (http_parser*)malloc(sizeof(http_parser));
    http_parser_init(parser, HTTP_REQUEST);
    parser->data = arg;
    if (event & EV_READ) {
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
    }
}

void makeConnection(int fd, short event, void* arg)
{
    if (event & EV_READ) {
        struct event* ev = static_cast<struct event*> (arg);
        struct sockaddr_in cliaddr;
        socklen_t len;
        int connfd = 0;
        if ((connfd = accept(fd, (struct sockaddr*)&cliaddr, &len)) < 0) {
            std::cerr << "Make connection error!" << std::endl;
            return;
        }

        fcntl(connfd, F_SETFL, O_NONBLOCK);
        struct event* connev = (struct event*)malloc(sizeof(struct event));
        event_set(connev, connfd, EV_READ | EV_WRITE | EV_PERSIST, onMessage, &connev);
        event_base_set(ev->ev_base, connev);
        event_add(connev, NULL);
    } else {
        std::cerr << "We do not care listener write or error!" << std::endl;
    }
}

int main(int argc, char** argv)
{
    struct event_base* base;
    base  = event_base_new();
    if (NULL == base) {
        std::cerr << "create event base error" << std::endl;
        return 0;
    }
    int listenfd = 0;
    listenfd = createSocketAndListen();
    if (listenfd < 0) {
        std::cerr << "Create and listen fd error!" << std::endl;
    }
    if (setNonBlock(listenfd) < 0) {
        std::cerr << "Set listenfd nonblocking error!" << std::endl;
    }
    struct event ev;
    event_set(&ev, listenfd, EV_READ |EV_PERSIST, makeConnection, &ev);
    event_base_set(base, &ev);
    event_add(&ev, NULL);
    event_base_dispatch(base);
    return 0;
}
