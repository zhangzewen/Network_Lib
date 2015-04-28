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
#include "http_parser.h"


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


int main(int argc, char** argv)
{
#if 0
    struct event_base* base;
    base  = event_base_new();
    if (NULL == base) {
        std::cerr << "create event base error" << std::endl;
        return 0;
    }
    int listenfd = 0;
    if (listenfd < 0) {
        std::cerr << "Create and listen fd error!" << std::endl;
    }
    struct event ev;
    event_set(&ev, listenfd, EV_READ |EV_PERSIST, makeConnection, &ev);
    event_base_set(base, &ev);
    event_add(&ev, NULL);
    event_base_dispatch(base);
#endif
    return 0;
}
