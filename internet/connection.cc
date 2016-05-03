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

Connection::Connection(int fd, struct event_base* base) : connfd_(fd),
    conn_state_(CON_DISCONNECTED), buf_(NULL), base_(base),
    keep_alived_(false), privdata_(NULL)
{
}

Connection::~Connection()
{
}

/**
  do some init work, such as init recv and send buffer, add read,write
  error event callback function and so on. when it done, enable read event
  */
void Connection::init()
{
    if (conn_state_ == CON_DISCONNECTED) {
        conn_state_ = CON_CONNECTING;
    }
    // first should init buffer
    // second set callbacks by add read/write event
    disp_->addReadEvent(connfd_, std::bind(&Connection::onReadEventCallback, this, _1));
    disp_->addWriteEvent(connfd_, std::bind(&Connection::onWriteEventCallback, this, _1));
    startRead();
}

/**
  wrapper function, get data from recv buffer and call customize callback
  function
  */
void Connection::onMessage()
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
int Connection::tryWrite()
{
    enableWrite();
    return 0;
}

/**
  static method that will be registed to bufferevent_setcb()
  */
void Connection::eventReadCallBack(bufferevent* buf, void* arg)
{
    assert(NULL != buf);
    Connection* conn = static_cast<Connection*>(arg);
    // read event fired, change conn_state to reading
    // conn->conn_state_ = CON_READING;
    conn->handleRead();  // handle read
}

/**
  @param
  */
void Connection::eventEmptyCallBack(bufferevent* buf, void* arg)
{
    // just doing nothing
    assert(NULL == buf);
    assert(NULL == arg);
}

/**
  @param
  */
void Connection::eventWriteCallBack(bufferevent* buf, void* arg)
{
    assert(NULL != buf);
    Connection* conn = static_cast<Connection*>(arg);
    conn->handleWrite();
}

/**
  @param
  */
void Connection::eventErrorCallBack(bufferevent* buf, short what, void* arg)
{
    assert(NULL != buf);
    assert(0 != what);
    Connection* conn = static_cast<Connection*>(arg);
    conn->handleError(conn, what);
}

/**
  @param
  */
void Connection::startRead()
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
void Connection::readDone()
{
}

/**
  @param
  */
void Connection::startWrite()
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
void Connection::writeDone()
{
    if (!isKeepAlived()) {
        // just free Connection
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
void Connection::handleRead()
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
            // close the Connection
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
void Connection::handleWrite()
{
    //if (!EVBUFFER_LENGTH(buf_->output)) {
    // return;
    //}

    if (customizeOnWriteCallBack_) {
        customizeOnWriteCallBack_(this);
    }
}


/**
  close Connection gently
  first, shutdown write
  second, wait for a read event fired until timeout, TODO SO_LINGER
  third, if read return 0. TCP close done, do the close work
  */
void Connection::closeConnection()  // 主动关闭连接
{
    if (conn_state_ == CON_DISCONNECTED) {
        return;
    }
    conn_state_ = CON_DISCONNECTING;
    shutdown(connfd_, SHUT_WR);
    setCustomizeOnMessageCallBack(
            std::bind(&Connection::lingeringClose, this, _1, _2, _3));
    enableRead();
}

/**
  force close connection,where there is a deadly error happened
  do not check where remote server/client send FIN
  just release connection and call close()
  */
void Connection::forceCloseConnection()
{
}

/**
  @param
  */
int Connection::doCloseConnection()
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
bool Connection::reuseConnection()
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
void Connection::setKeepAlived(bool isKeepAlived)
{
    keep_alived_ = isKeepAlived;
}

/**
  @param
  */
bool Connection::isKeepAlived()
{
    return keep_alived_;
}

/**
  @param
  */
void Connection::setListener(Listener* listen)
{
    listener_ = listen;
}

/**
  @param
  */

/**
  @param
  */
void Connection::setPrivData(void* data) {
    privdata_ = data;
}

/**
  @param
  */
void* Connection::getPrivData() const {
    return privdata_;
}


/**
  @param
  */
int Connection::doWrite(const char* buf, int len)
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
int Connection::setRemoteAddr(struct sockaddr_in* remoteAddr, socklen_t len)
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
    connection* con = new connection(fd, base);
    return 0;
}

void Connection::lingeringClose(Connection* conn, char* buf, int len)
{
    assert(conn);
    assert(buf);
    if (len == 0) {
        doCloseConnection();
    }
}
void Connection::onReadEventCallback(std::shared_ptr<Event> ev)
{
    // check if timeout fire this callback
    // check if ev has data to read
    if (!ev->isActive()) {

    }

    if (!ev->isReadReady()) {
        //  if read event is not ready ,just return
        return;
    }

    // do the read
    // should be a buffer to do this work which wrap the system call read
    if (customizeOnMessageCallBack_) {
        customizeOnMessageCallBack_();
    }
}

void Connection::onWriteEventCallback(std::shared_ptr<Event> ev)
{
    if (ev->isActive() && ev->isReadReady()) {
    }

}
