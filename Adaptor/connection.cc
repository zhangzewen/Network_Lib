#include "connection.h"
#include <event.h>
#include <stdio.h>
#include <assert.h>
//#include "http_parser.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <strings.h>
#include "event.h"
#include "listener.h"

connection::connection(int fd, struct event_base* base) : connfd_(fd),
	buf_(NULL), base_(base), privdata_(NULL)
{
}

connection::~connection()
{
}

bool connection::init()
{
    buf_ = bufferevent_new(connfd_, eventReadCallBack, eventWriteCallBack, eventErrorCallBack, this);
    if (NULL == buf_) {
        std::cout << "bufferevent malloc error!" << std::endl;
        return false;
    }
    bufferevent_base_set(base_, buf_);
    //bufferevent_enable(buf_, EV_READ);
    //bufferevent_setcb(buf_, eventReadCallBack, NULL, NULL, this);
    return true;
}

connection::CONN_STATE connection::onMessage()
{
#if 0
    int nparsed = 0;
    char buffer[4096] = {0};
    int nread = bufferevent_read(buf_, buffer, 4095);
    nparsed = http_parser_execute(parser_, settings_, buffer, nread);
    if (parser_->upgrade) {
        //do something
    } else if (nparsed != nread) {
        //Handle error. Usually just close the connection.
		parse_state_ = PARSEERROR;
    }
	// 这边应该是处理adaptor的
    //if (http_body_is_final(parser_)) { // 这个connection 已经完成
    //    std::cerr << "http body parser is done" << std::endl;
	//   return READDONE;
    //}

	switch(parse_state_) {
		case PARSEING:
			return READING;
		case PARSEERROR:
			return READERROR;
		default:
			break;
	}
#endif
	// this is ugly now,  there must will be a recv buffer conf
	char buffer[4096] = {0};
	int nread = bufferevent_read(buf_, buffer, 4095);
	CONN_STATE ret;
	if (customizeOnMessageCallBack_) {
		ret = customizeOnMessageCallBack_(this, buffer, nread);
	}

    return ret;
}

void connection::eventReadCallBack(bufferevent* buf, void* arg)
{
    connection* conn = static_cast<connection*>(arg);
	conn->conn_state_ = CON_READING; // read event fired, change conn_state to reading
    conn->handleRead();//handle read
}

void connection::eventEmptyCallBack(bufferevent* buf, void* arg)
{
    // just doing nothing
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

void connection::handleRead()
{
    switch(conn_state_) {
        case CON_READING:
			conn_state_ = onMessage();
            break;
        case CON_READERROR:
			//主动关闭连接
            doCloseConnection();
            break;
        case CON_READDONE:
			//here we just make read event hanld empty, just let the TCP/IP recv data,
			//another thing is: for http,we just support keepalive ,not pipeline
			//when reading done,let's process the request,there will be a handler for customizeing
			//and when read done,let the process begin
            bufferevent_setcb(buf_, eventEmptyCallBack, NULL, NULL, NULL);
            bufferevent_enable(buf_, EV_READ);
			conn_state_ = CON_PROCESSING;
            break;
        case CON_PROCESSING:
            // the request is processing
            break;
        case CON_PROCESSERROR:
            break;
        case CON_PROCESSDONE:
            break;
        case CON_WRITTING:
            break;
		case CON_WRITEERROR:
            break;
		case CON_WRITEDONE:
			break;
        default:
            break;
    }
}

void connection::handleWrite()
{
}

void connection::handleError()
{
}
void connection::handleTimeOut()
{
}

int connection::closeConnection() //主动关闭连接
{

	if (conn_state_ == CON_DISCONNECTED) { //如果连接状态已经是关闭的，则不做任何处理，直接返回
		return 0;
	}
	conn_state_ = CON_DISCONNECTING; //step 1. close read
	bufferevent_disable(buf_, EV_READ);
	shutdown(connfd_, SHUT_RD);
	//step 2. 如果buffer中还有数据没有发送完，发送完数据，并重新设置超时时间
	if (buf_->output->off) {
		bufferevent_setcb(buf_, NULL, eventWriteCallBack, NULL, this);
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
    //step3. close connfd
    close(connfd_);
}

bool connection::reuseConnection()
{
    keep_alived_ = false;
    listener_ = NULL;
    conn_state_ = CON_CONNECTING;
    connfd_ = -1;
}

void connection::setKeepAlived(bool isKeepAlived)
{
    keep_alived_ = isKeepAlived;
}
void connection::setListener(listener* listen)
{
    listener_ = listen;
}

connection::CONN_STATE process(connection* conn)
{

}

bool connection::reuseEvBuffer(struct evbuffer* buf)
{
    if (NULL == buf) {
        return true;
    }
    buf->buffer = buf->orig_buffer;
    buf->misalign = 0;;
    buf->off = 0;
    buf->cb = NULL;
    buf->cbarg = NULL;
}

bool connection::reuseEvBuffer(struct evbuffer_chain* chain)
{
    if (NULL == chain) {
        return true;
    }

    struct evbuffer_chain* next = NULL;
    next = chain->next;
    for (;next;next = chain->next) {
        reuseEvBufferChain(next);
    }
}

bool connection::reuseEvBufferChain(struct evbuffer_chain* chain)
{
    if (NULL == buf) {
        return true;
    }
    buf->buffer = buf->orig_buffer;
    buf->misalign = 0;;
    buf->off = 0;
    buf->cb = NULL;
    buf->cbarg = NULL;
}

bool connection::reuseBufferEvent(struct bufferevent* bufev)
{
    event_del(&bufev->ev_read);
    event_del(&bufev->ev_write);
    reuseEvBuffer(bufev->input);
    reuseEvBuffer(bufev->output);
    return true;
}

short connection::bufferevent_get_enabled(struct bufferevent* bufev)
{
    return bufev->enabled;
}
void connection::EnableRead() {
    int event = bufferevent_get_enabled(buf_);
    if (event & EV_READ)
        return;
    bufferevent_enable(buf_, EV_READ);
}

void connection::EnableWrite() {
    int event = bufferevent_get_enabled(buf_);
    if (event & EV_WRITE)
        return;
    bufferevent_enable(buf_, EV_WRITE);
}

void connection::DisableRead() {
    int event = bufferevent_get_enabled(buf_);
    if (event & EV_READ)
        bufferevent_disable(buf_, EV_READ);
}

void connection::DisableWrite() {
    int event = bufferevent_get_enabled(buf_);
    if (event & EV_WRITE)
        bufferevent_disable(buf_, EV_WRITE);
}

void connection::DisableReadWrite() {
    int old = bufferevent_get_enabled(buf_);
    if (old)
        bufferevent_disable(buf_, old);
}

void connection::setPrivData(void* data) {
	privdata_ = data;
}

void* connection::getPrivData() const {
	return privdata_;
}

