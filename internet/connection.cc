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
#include <glog/logging.h>
#include "listener.h"
#include "connection.h"
#include "util.h"

using namespace std::placeholders;

connection::connection(int fd, struct event_base* base) : connfd_(fd),
  conn_state_(CON_CONNECTING), buf_(NULL), base_(base),
  keep_alived_(false), privdata_(NULL)
{
}

connection::~connection()
{
}

/**
  do some init work, such as init recv and send buffer, add read,write
  error event callback function and so on. when it done, enable read event
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
  wrapper function, get data from recv buffer and call customize callback
  function
*/
void connection::onMessage()
{
  // this is ugly now,  there must will be a recv buffer conf
  // if input size is empty, just return reading
  //
  //  if (!EVBUFFER_LENGTH(buf_->input)) {
  //    conn_state_ = CON_READING;
  //    return;
  //  }
  char buffer[4096] = {0};
  int nread = bufferevent_read(buf_, buffer, 4095);
  if (customizeOnMessageCallBack_) {
    customizeOnMessageCallBack_(this, buffer, nread);
  }

  return;
}

/**
  try to write and regist the write event if it's not registed
*/
int connection::tryWrite()
{
  enableWrite();
  return 0;
}

/**
  static method that will be registed to bufferevent_setcb()
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
    LOG(INFO) << "keepalived: " << isKeepAlived() << ", just release the connection";
    closeConnection();
    return;
  }
  // this connnection is keepalived,just reset buffers and enable read ,
  // wait reading
  LOG(INFO) << " connection keepalived, wait for another connection";
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
  } else if (what == (EVBUFFER_READ | EVBUFFER_EOF)) {  //  recv remote FIN
    if (customizeOnConnectionCloseCallBack_) {
      customizeOnConnectionCloseCallBack_(conn);
    }
    closeConnection();
  }
}

/**
  close connection gently
  first, shutdown write
  second, wait for a read event fired until timeout, TODO SO_LINGER
  third, if read return 0. TCP close done, do the close work
*/
void connection::closeConnection()  // 主动关闭连接
{
  if (conn_state_ == CON_DISCONNECTED) {
    return;
  }
  conn_state_ = CON_DISCONNECTING;
  shutdown(connfd_, SHUT_WR);
  setCustomizeOnMessageCallBack(
      std::bind(&connection::lingeringClose, this, _1, _2, _3));
  enableRead();
}

/**
  force close connection,where there is a deadly error happened
  do not check where remote server/client send FIN
  just release connection and call close()
*/
void connection::forceCloseConnection()
{
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
void connection::setListener(Listener* listen)
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
  when accept done and create connection successful, we get the remote ip and port

  @param remoteAddr a type of struct sockaddr_in pointer point to addr which  from the 2th parameter of accpet system call
*/
int connection::setRemoteAddr(struct sockaddr_in* remoteAddr, socklen_t len)
{
  assert(remoteAddr);
  assert(len);
  //  char remoteaddr[256] = {0};
  //  char* ret = NULL;
  //  ret = inet_ntop(AF_INET, (const void*)(&(remoteAddr->sin_addr)), remoteaddr, len);
  //  if (NULL == ret) {
  //    LOG(ERROR) << "get remote address error: " << strerror(errno);
  //    return -1;
  //  }
  //  remote_addr_ = remoteaddr;
  //  remote_addr_.append(to_string(remoteAddr->sin_port));
  return 0;
}

void connection::lingeringClose(connection* conn, char* buf, int len)
{
  assert(conn);
  assert(buf);
  if (len == 0) {
    doCloseConnection();
  }
}
