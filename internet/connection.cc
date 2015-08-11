#include "connection.h"
#include <event.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <strings.h>
#include <assert.h>
#include "event.h"
#include "listener.h"

connection::connection(int fd, struct event_base* base) : connfd_(fd), conn_state_(CON_CONNECTING),
  buf_(NULL), base_(base), keep_alived_(false), privdata_(NULL)
{
}

connection::~connection()
{
}

// just do some init work, and begin to register callback handle
void connection::init()
{
  if (NULL == buf_) {
    buf_ = bufferevent_new(connfd_, NULL, NULL, NULL, this);
    if ( NULL == buf_) {
      //TODO: close the connection 
    }
  } 
  resetEvBuffer(buf_->input);
  resetEvBuffer(buf_->output);
  bufferevent_base_set(base_, buf_);
  bufferevent_setcb(buf_, eventReadCallBack, NULL, NULL, this);
  if (conn_state_ == CON_CONNECTING) {
    conn_state_ = CON_CONNECTED;
  } 
  //bufferevent_enable(buf_, EV_READ);
  startRead();
}

connection::CONN_STATE connection::onMessage()
{
  // this is ugly now,  there must will be a recv buffer conf
  // if input size is empty, just return reading
  if (!EVBUFFER_LENGTH(buf_->input)) {
    return CON_READING;
  }
  char buffer[4096] = {0};
  int nread = bufferevent_read(buf_, buffer, 4095);
  CONN_STATE ret;
  if (customizeOnMessageCallBack_) {
    ret = customizeOnMessageCallBack_(this, buffer, nread);
  }

#if 0
  if (ret == CON_READDONE) {
    ret = doProcess();
  }
#endif
  return ret;
}

#if 0
connection::CONN_STATE connection::doProcess()
{
  CONN_STATE ret;
  if (customizeOnProcessCallBack_) {
    ret = customizeOnProcessCallBack_(this);
  }
  
  if (ret == CON_PROCESSDONE) {
    tryWrite();
  }
  // when there is no customize process callback just change the states from CON_PROCESSING to CON_PROCESSDONE
  return CON_PROCESSDONE;
}
#endif

// try to write and regist the write event if it's not registed
int connection::tryWrite()
{
  // if output buffer is empty just return -1 
  if (!EVBUFFER_LENGTH(buf_->output)) {
    return -1;
  }
  enableWrite();
  return 0;
}

void connection::eventReadCallBack(bufferevent* buf, void* arg)
{
  assert(NULL != buf);
  connection* conn = static_cast<connection*>(arg);
  //conn->conn_state_ = CON_READING; // read event fired, change conn_state to reading
  conn->handleRead();//handle read
}

void connection::eventEmptyCallBack(bufferevent* buf, void* arg)
{
  // just doing nothing
  assert(NULL == buf);
  assert(NULL == arg);
}

void connection::eventWriteCallBack(bufferevent* buf, void* arg)
{
  assert(NULL == buf);
  connection* conn = static_cast<connection*>(arg);
  conn->handleWrite();
}

void connection::eventErrorCallBack(bufferevent* buf, short event, void* arg)
{
  assert(NULL == buf);
  assert(0 != event);
  connection* conn = static_cast<connection*>(arg);
  conn->handleError();
}

#if 0 
// for read and write timeout
void connection::eventReadTimeoutCallBack(bufferevent* buf, void* arg)
{
  assert(NULL == buf);
  connection* conn = static_cast<connection*>(arg);
  conn->handleTimeOut();
}

void connection::eventWriteTimeoutCallBack(bufferevent* buf, void* arg)
{
  assert(NULL == buf);
  connection* conn = static_cast<connection*>(arg);
  conn->handleTimeOut();
}
#endif


//
void connection::startRead()
{
  //assert(buf_->input);
  disableWrite();//close write event
  enableRead();// enable read event

  if (EVBUFFER_LENGTH(buf_->input)) {
    handleRead();
  }
}

void connection::readDone()
{
}

void connection::startWrite()
{
  if (!EVBUFFER_LENGTH(buf_->output)) {
    // write buff is empty 
    // just return
    conn_state_ = CON_WRITTING;
    return ;
  }

  conn_state_ = CON_WRITTING;
  //int ret = tryWrite();
  //TODO: write data 
  if (!EVBUFFER_LENGTH(buf_->output)) {
    // write done
    writeDone();
    return ;
  }
  enableWrite();
  return ;
}

void connection::writeDone()
{
  if(!isKeepAlived()) {
    //just free connection
    closeConnection();
  }
  // this connnection is keepalived,just reset buffers and enable read ,wait reading
  init();
  //startRead();
  return ;
}



void connection::handleRead()
{
  switch(conn_state_) {
    case CON_CONNECTED:
    //case CON_READING:
    case CON_IDLE:
      conn_state_ = onMessage();
      break;
#if 0
    case CON_READERROR:
      //主动关闭连接
      doCloseConnection();
      break;
    case CON_READDONE:
      //here we just make read event handle empty, just let the TCP/IP recv data,
      //another thing is: for http,we just support keepalive ,not pipeline
      //when reading done,let's process the request,there will be a handler for customizeing
      //and when read done,let the process begin
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
    //case CON_WRITEERROR:
    //  break;
    //case CON_WRITEDONE:
    //  break;
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

// this function has some errors
int connection::closeConnection() //主动关闭连接
{
  if (conn_state_ == CON_DISCONNECTED) { // if the connection is already closed ,just doing nothing
    return 0;
  }
  conn_state_ = CON_DISCONNECTING; //step 1. shut down Write ,send FIN
  //bufferevent_disable(buf_, EV_WRITE);
  //bufferevent_disable(buf_, EV_READ);
  bufferevent_free(buf_);
#if 0
  shutdown(connfd_, SHUT_WR);
  //step 2. 如果buffer中还有数据没有发送完，发送完数据，并重新设置超时时间
  if (buf_->output->off) {
    bufferevent_setcb(buf_, NULL, eventWriteCallBack, NULL, this);
    bufferevent_enable(buf_, EV_WRITE);
  }
#endif
  close(connfd_);
  return 0;
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
  return 0;
}

bool connection::reuseConnection()
{
  keep_alived_ = false;
  listener_ = NULL;
  conn_state_ = CON_CONNECTING;
  connfd_ = -1;
  return true;
}

void connection::setKeepAlived(bool isKeepAlived)
{
  keep_alived_ = isKeepAlived;
}

bool connection::isKeepAlived()
{
  return keep_alived_;
}

void connection::setListener(listener* listen)
{
  listener_ = listen;
}

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

bool connection::reuseBufferEvent(struct bufferevent* bufev)
{
  event_del(&bufev->ev_read);
  event_del(&bufev->ev_write);
  resetEvBuffer(bufev->input);
  resetEvBuffer(bufev->output);
  return true;
}

short connection::bufferevent_get_enabled(struct bufferevent* bufev)
{
  return bufev->enabled;
}

void connection::enableRead() {
  int event = bufferevent_get_enabled(buf_);
  if (event & EV_READ)
    return;
  bufferevent_enable(buf_, EV_READ);
}

void connection::enableWrite() {
  int event = bufferevent_get_enabled(buf_);
  if (event & EV_WRITE)
    return;
  bufferevent_enable(buf_, EV_WRITE);
}

void connection::disableRead() {
  int event = bufferevent_get_enabled(buf_);
  if (event & EV_READ)
    bufferevent_disable(buf_, EV_READ);
}

void connection::disableWrite() {
  int event = bufferevent_get_enabled(buf_);
  if (event & EV_WRITE)
    bufferevent_disable(buf_, EV_WRITE);
}

void connection::disableReadWrite() {
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


int connection::doWrite(char* buf, int len)
{
}
