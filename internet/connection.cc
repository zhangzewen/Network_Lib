// Copyright [2015] <Zhang Zewen>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <iostream>
#include <event.h>
#include <glog/logging.h>
#include "listener.h"
#include "connection.h"
#include "util.h"

connection::connection(int fd, struct event_base* base) : connfd_(fd),
  conn_state_(CON_CONNECTING), buf_(NULL), base_(base),
  keep_alived_(false), privdata_(NULL)
{
}

connection::~connection()
{
}

// just do some init work, and begin to register callback handle
/**
  @param
*/
void connection::init()
{
  if (NULL == buf_) {
    buf_ = bufferevent_new(connfd_, NULL, NULL, NULL, this);
    if (NULL == buf_) {
      // close the connection
    }
  }
  resetEvBuffer(buf_->input);
  resetEvBuffer(buf_->output);
  bufferevent_base_set(base_, buf_);
  bufferevent_setcb(buf_,
    eventReadCallBack,
    eventWriteCallBack,
    eventErrorCallBack,
    this);
  if (conn_state_ == CON_CONNECTING) {
    conn_state_ = CON_CONNECTED;
  }
  // bufferevent_enable(buf_, EV_READ);
  startRead();
}

/**
  @param
*/
void connection::onMessage()
{
  // this is ugly now,  there must will be a recv buffer conf
  // if input size is empty, just return reading
  if (!EVBUFFER_LENGTH(buf_->input)) {
    conn_state_ = CON_READING;
    return;
  }
  char buffer[4096] = {0};
  int nread = bufferevent_read(buf_, buffer, 4095);
  if (customizeOnMessageCallBack_) {
    customizeOnMessageCallBack_(this, buffer, nread);
  }

  return;
}

// try to write and regist the write event if it's not registed
/**
  @param
*/
int connection::tryWrite()
{
  // if output buffer is empty just return -1
  //if (!EVBUFFER_LENGTH(buf_->output)) {
   // return -1;
 // }
  enableWrite();
  return 0;
}

/**
  @param
*/
void connection::eventReadCallBack(bufferevent* buf, void* arg)
{
  assert(NULL != buf);
  connection* conn = static_cast<connection*>(arg);
  // read event fired, change conn_state to reading
  // conn->conn_state_ = CON_READING;
  conn->handleRead();  // handle read
}

/**
  @param
*/
void connection::eventEmptyCallBack(bufferevent* buf, void* arg)
{
  // just doing nothing
  assert(NULL == buf);
  assert(NULL == arg);
}

/**
  @param
*/
void connection::eventWriteCallBack(bufferevent* buf, void* arg)
{
  assert(NULL != buf);
  connection* conn = static_cast<connection*>(arg);
  conn->handleWrite();
}

/**
  @param
*/
void connection::eventErrorCallBack(bufferevent* buf, short what, void* arg)
{
  assert(NULL != buf);
  assert(0 != what);
  connection* conn = static_cast<connection*>(arg);
  conn->handleError(conn, what);
}

/**
  @param
*/
void connection::startRead()
{
  // assert(buf_->input);
  disableWrite();  // close write event
  enableRead();  // enable read event

  if (EVBUFFER_LENGTH(buf_->input)) {
    handleRead();
  }
}

/**
  @param
*/
void connection::readDone()
{
}

/**
  @param
*/
void connection::startWrite()
{
  if (!EVBUFFER_LENGTH(buf_->output)) {
    // write buff is empty
    // just return
    conn_state_ = CON_WRITTING;
    return;
  }

  conn_state_ = CON_WRITTING;
  // int ret = tryWrite();
  // write data
  if (!EVBUFFER_LENGTH(buf_->output)) {
    // write done
    writeDone();
    return;
  }
  enableWrite();
  return;
}

/**
  @param
*/
void connection::writeDone()
{
  if (!isKeepAlived()) {
    // just free connection
    closeConnection();
  }
  // this connnection is keepalived,just reset buffers and enable read ,
  // wait reading
  init();
  // startRead();
}



/**
  @param
*/
void connection::handleRead()
{
  switch (conn_state_) {
    case CON_CONNECTED:
    case CON_READING:
    case CON_IDLE:
      onMessage();
#if 0
    case CON_READERROR:
      // 主动关闭连接
      doCloseConnection();
      break;
    case CON_READDONE:
      // here we just make read event handle empty,
      // just let the TCP/IP recv data,
      // another thing is: for http,we just support keepalive ,not pipeline
      // when reading done,let's process the request,there will be a handler
      // for customizeing and when read done,let the process begin
      bufferevent_setcb(buf_, eventEmptyCallBack, NULL, NULL, NULL);
      enableRead();
      conn_state_ = CON_PROCESSING;
      break;
    case CON_PROCESSING:
      // the request is processing
      conn_state_ = doProcess();
      break;
    case CON_PROCESSERROR:
      // close the connection
      doCloseConnection();
      break;
    case CON_PROCESSDONE:
      tryWrite();
      break;
#endif
    case CON_WRITTING:
      break;
    // case CON_WRITEERROR:
    // break;
    // case CON_WRITEDONE:
    // break;
    default:
      break;
  }
}

/**
  @param
*/
void connection::handleWrite()
{
  //if (!EVBUFFER_LENGTH(buf_->output)) {
   // return;
  //}
  
  if (customizeOnWriteCallBack_) {
    customizeOnWriteCallBack_(this);
  }
}


/**
  @param
*/
void connection::handleError(connection* conn, short what)
{
  assert(NULL != conn);
  assert(0 != what);
  if (what & EVBUFFER_ERROR) {
    // just close connection
    closeConnection();
  } else if (what == (EVBUFFER_READ | EVBUFFER_TIMEOUT)) {  // read timeout
  } else if (what == (EVBUFFER_WRITE | EVBUFFER_TIMEOUT)) {  // write time out
  } else if (what == (EVBUFFER_READ | EVBUFFER_EOF)) {
    if (customizeOnConnectionCloseCallBack_) {
      customizeOnConnectionCloseCallBack_(conn);
    }
    closeConnection();
  }
}

// this function has some errors
/**
  @param
*/
int connection::closeConnection()  // 主动关闭连接
{
  // if the connection is already closed ,just doing nothing
  if (conn_state_ == CON_DISCONNECTED) {
    return 0;
  }
  conn_state_ = CON_DISCONNECTING;  // step 1. shut down Write ,send FIN
  // bufferevent_disable(buf_, EV_WRITE);
  // bufferevent_disable(buf_, EV_READ);
  bufferevent_free(buf_);
#if 0
  shutdown(connfd_, SHUT_WR);
  // step 2. 如果buffer中还有数据没有发送完，发送完数据，并重新设置超时时间
  if (buf_->output->off) {
    bufferevent_setcb(buf_, NULL, eventWriteCallBack, NULL, this);
    bufferevent_enable(buf_, EV_WRITE);
  }
#endif
  close(connfd_);
  return 0;
}

/**
  @param
*/
int connection::doCloseConnection()
{
  // step 1. delete read and write event
  // bufferevent_disable(buf_, EV_READ);
  // bufferevent_disable(buf_, EV_WRITE);
  // just use bufferevent_free is ok
  bufferevent_free(buf_);
  // step2. freep http_parser
  // step3. close connfd
  close(connfd_);
  return 0;
}

/**
  @param
*/
bool connection::reuseConnection()
{
  keep_alived_ = false;
  listener_ = NULL;
  conn_state_ = CON_CONNECTING;
  connfd_ = -1;
  return true;
}

/**
  @param
*/
void connection::setKeepAlived(bool isKeepAlived)
{
  keep_alived_ = isKeepAlived;
}

/**
  @param
*/
bool connection::isKeepAlived()
{
  return keep_alived_;
}

/**
  @param
*/
void connection::setListener(listener* listen)
{
  listener_ = listen;
}

/**
  @param
*/
bool connection::resetEvBuffer(struct evbuffer* buf)
{
  if (NULL == buf) {
    return false;
  }
  buf->buffer = buf->orig_buffer;
  buf->misalign = 0;;
  buf->off = 0;
  buf->cb = NULL;
  buf->cbarg = NULL;
  return true;
}

/**
  @param
*/
bool connection::reuseBufferEvent(struct bufferevent* bufev)
{
  event_del(&bufev->ev_read);
  event_del(&bufev->ev_write);
  resetEvBuffer(bufev->input);
  resetEvBuffer(bufev->output);
  return true;
}

/**
  @param
*/
short connection::bufferevent_get_enabled(struct bufferevent* bufev)
{
  return bufev->enabled;
}

/**
  @param
*/
void connection::enableRead() {
  int event = bufferevent_get_enabled(buf_);
  if (event & EV_READ)
    return;
  bufferevent_enable(buf_, EV_READ);
}

/**
  @param
*/
void connection::enableWrite() {
  int event = bufferevent_get_enabled(buf_);
  if (event & EV_WRITE)
    return;
  bufferevent_enable(buf_, EV_WRITE);
}

/**
  @param
*/
void connection::disableRead() {
  int event = bufferevent_get_enabled(buf_);
  if (event & EV_READ)
    bufferevent_disable(buf_, EV_READ);
}

/**
  @param
*/
void connection::disableWrite() {
  int event = bufferevent_get_enabled(buf_);
  if (event & EV_WRITE)
    bufferevent_disable(buf_, EV_WRITE);
}

/**
  @param
*/
void connection::disableReadWrite() {
  int old = bufferevent_get_enabled(buf_);
  if (old)
    bufferevent_disable(buf_, old);
}

/**
  @param
*/
void connection::setPrivData(void* data) {
  privdata_ = data;
}

/**
  @param
*/
void* connection::getPrivData() const {
  return privdata_;
}


/**
  @param
*/
int connection::doWrite(const char* buf, int len)
{
  assert(NULL != buf);
  int ret = 0;
  ret = bufferevent_write(buf_, buf, len);
  return ret;
}


/**
*/

int connection::setRemoteAddr(struct sockaddr_in* remoteAddr, socklen_t len)
{
  assert(remoteAddr);
  assert(len);
  //  char remote[256] = {0};
  //  char* ret = NULL;
  //  ret = inet_ntop(AF_INET, (const void*)(&(remoteAddr->sin_addr)), const_cast<char*>(remote), len);
  //  if (NULL == ret) {
  //    LOG(ERROR) << "get remote address error: " << strerror(errno);
  //    return -1;
  //  }
  //  remote_addr_ = remote;
  //  remote_addr_.append(to_string(remoteAddr->sin_port));
  return 0;
}
