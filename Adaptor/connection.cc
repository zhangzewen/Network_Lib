#include "connection.h"
#include <event.h>
#include <stdio.h>
#include <assert.h>
#include "http_parser.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include "event.h"

connection::connection(int fd, struct event_base* base) : connfd_(fd),
    read_state_(READING), buf_(NULL), parser_(NULL), settings_(NULL),
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
    fprintf(stderr, "len: %d", len);
	return 0;
}

int dontcall_header_value_cb (http_parser *p, const char *buf, size_t len)
{
    assert( p != NULL);
    assert( buf != NULL);
    assert( len != 0);
    fprintf(stderr, "\n\n*** on_header_field() called on paused parser ***\n\n");
    fprintf(stderr, "len: %d", len);

	return 0;
}

int dontcall_request_url_cb (http_parser *p, const char *buf, size_t len)
{
  if (p || buf || len) { } // gcc
  fprintf(stderr, "\n\n*** on_request_url() called on paused parser ***\n\n");
	char buffer[1024] = {0};
	snprintf(buffer,len + 1, " %s", buf);
    fprintf(stderr, "url: %s\n", buffer);
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
    if (http_should_keep_alive(p) == 0) {
        std::cerr << "connection close" << std::endl;
        connection* conn = static_cast<connection*>(p->data);
        conn->doCloseConnection();
    }
	return 0;
}


bool connection::init()
{
    buf_ = bufferevent_new(connfd_, eventReadCallBack, eventWriteCallBack, eventErrorCallBack, this);
    if (NULL == buf_) {
        std::cout << "bufferevent malloc error!" << std::endl;
        return false;
    }
    bufferevent_base_set(base_, buf_);
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
    bufferevent_setcb(buf_, eventReadCallBack, NULL, NULL, this);
    return true;
}

connection::READ_STATE connection::onMessage()
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
	// 这边应该是处理adaptor的
    if (http_body_is_final(parser_)) { // 这个connection 已经完成
        std::cerr << "http body parser is done" << std::endl;
	    return READDONE;
    }
    return READING;
}

void connection::eventReadCallBack(bufferevent* buf, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
    conn->handleRead();
}
void connection::eventWriteCallBack(bufferevent* buf, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
    conn->handleWrite();
}
void connection::eventErrorCallBack(bufferevent* buf, short event, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
    conn->handleError();
}
void connection::eventTimeoutCallBack(bufferevent* buf, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
    conn->handleTimeOut();
}

int connection::handleRead()
{
    switch(read_state_) {
        case READING:
            read_state_ = onMessage();
            break;
        case READERROR:
			//主动关闭连接
            doCloseConnection();
            break;
        case READDONE:
			//处理业务逻辑
            break;
        default:
            break;
    }
}

int connection::handleWrite()
{
}

int connection::handleError()
{
}
int connection::handleTimeOut()
{
}

int connection::CloseConnection() //主动关闭连接
{

	if (conn_state_ == DISCONNECTED) { //如果连接状态已经是关闭的，则不做任何处理，直接返回
		return 0;
	}
	conn_state_ = DISCONNECTING;
	//step 1. close read
	bufferevent_disable(buf_, EV_READ);
	shutdown(connfd_, SHUT_RD);
	//step 2. 如果buffer中还有数据没有发送完，发送完数据，并重新设置超时时间
	if (buf_->output->off) {
		bufferevent_setcb(buf_, NULL, NULL, NULL, this);
		bufferevent_enable(buf_, EV_WRITE);
	}
}

int connection::doCloseConnection()
{
    //step 1. delete read and write event
    //bufferevent_disable(buf_, EV_READ);
    //bufferevent_disable(buf_, EV_WRITE);
    //just use bufferevent_free is ok
    bufferevent_free(buf_);
    //step2. freep http_parser
    free(settings_);
    free(parser_);
    //step3. close connfd
    close(connfd_);
}
