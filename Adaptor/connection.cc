#include "connection.h"
#include <event.h>
#include <stdio.h>
#include <assert.h>
#include "http_parser.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

connection::connection(int fd, struct event_base* base) : connfd_(fd),
    read_state_(READBEGIN), buf_(NULL), parser_(NULL), settings_(NULL),
    base_(base)
{
}

connection::~connection()
{
}

int dontcall_message_begin_cb (http_parser *p)
{
	if (p) {

	}
  fprintf(stderr, "\n\n*** on_message_begin() called on paused parser ***\n\n");
	return 0;
}

int dontcall_header_field_cb (http_parser *p, const char *buf, size_t len)
{
	assert( p != NULL);
	assert( buf != NULL);
	assert( len != 0);
    fprintf(stderr, "\n\n*** on_header_field() called on paused parser ***\n\n");
	return 0;
}

int dontcall_header_value_cb (http_parser *p, const char *buf, size_t len)
{
    assert( p != NULL);
    assert( buf != NULL);
    assert( len != 0);
    fprintf(stderr, "\n\n*** on_header_field() called on paused parser ***\n\n");

	return 0;
}

int dontcall_request_url_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_request_url() called on paused parser ***\n\n");
	char buffer[1024] = {0};
	sprintf(buffer, buf, len);
	return 0;
}

int dontcall_body_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_body_cb() called on paused parser ***\n\n");
  fprintf(stderr, "%s", buf);
	return 0;
}

int dontcall_headers_complete_cb (http_parser *p)
{
  if (p) { } // gcc
  fprintf(stderr, "\n\n*** on_headers_complete() called on paused "
                  "parser ***\n\n");
	return 0;
}

int dontcall_message_complete_cb (http_parser *p)
{
  fprintf(stderr, "\n\n*** on_message_complete() called on paused "
                  "parser ***\n\n");
	return 0;
}


bool connection::init()
{
    buf_ = (bufferevent*)malloc(sizeof(bufferevent));
    if (NULL == buf_) {
        std::cout << "bufferevent malloc error!" << std::endl;
        return false;
    }
    bufferevent_base_set(base_, buf_);
    bufferevent_setcb(buf_, eventReadCallBack, eventWriteCallBack, eventErrorCallBack, this);
    settings_ = (http_parser_settings*)malloc(sizeof(http_parser_settings));
    settings_->on_message_begin = ::dontcall_message_begin_cb;
    settings_->on_header_field = ::dontcall_header_field_cb;
    settings_->on_header_value = ::dontcall_header_value_cb;
    settings_->on_url = ::dontcall_request_url_cb;
    settings_->on_body = ::dontcall_body_cb;
    settings_->on_headers_complete = ::dontcall_headers_complete_cb;
    settings_->on_message_complete = ::dontcall_message_complete_cb;

	parser_ = (http_parser*)malloc(sizeof(http_parser));
    if (NULL == parser_) {
        return false;
    }
    http_parser_init(parser_, HTTP_REQUEST);
    parser_->data = static_cast<void*>(this);
    bufferevent_enable(buf_, EV_READ);
}

int connection::onMessage()
{
    int nparsed = 0;
    char buffer[4096] = {0};
    int nread = bufferevent_read(buf_, buffer, 4095);
    nparsed = http_parser_execute(parser_, settings_, buffer, nread);
    if (parser_->upgrade) {
        //do something
    } else if (nparsed != nread) {
        //Handle error. Usually just close the connection.
    }
}

void connection::eventReadCallBack(bufferevent* buf, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
    conn->handleRead();
}
void connection::eventWriteCallBack(bufferevent* buf, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
    conn->handleRead();
}
void connection::eventErrorCallBack(bufferevent* buf, short event, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
    conn->handleRead();
}
void connection::eventTimeoutCallBack(bufferevent* buf, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
    conn->handleRead();
}

int connection::handleRead()
{
    switch(read_state_) {
        case READBEGIN:
            read_state_ = onMessage();
            break;
        case READING:
            break;
        case READERROR:
            close(connfd_);
            break;
        case READDONE:
            break;
        default:
            break;
    }
}
