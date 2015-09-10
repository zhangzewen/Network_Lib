// Copyright [2015] <Zhang Zewen>

#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <event.h>
#include <glog/logging.h>
#include "listener.h"
#include "connection.h"
#include "util.h"

listener::listener(const std::string& host, int port,
  struct event_base* base) : host_(host), port_(port), base_(base)
{
}

listener::listener(const std::string& host, int port) : host_(host),
  port_(port), base_(NULL)
{
}

/**
  @param fd 
  @param event 
  @param arg an argument to be passed to the callback function
*/
void listener::listenCallBack(int fd, short event, void* arg)
{
  assert(fd > 0);
  assert(fd & EV_READ);
  assert(NULL != arg);

  listener* listen = static_cast<listener*>(arg);
  if (event & EV_READ) {
    do {
      struct sockaddr_in cliaddr;
      socklen_t len;
      int connfd = 0;
      if ((connfd = accept(fd, (struct sockaddr*)&cliaddr, &len)) < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          LOG(ERROR) << "accept() not ready";
          return;
        }
        LOG(ERROR) << "accept error!";
        return;
      }
      listen->doMakeConnection(connfd);
    }while(true);
  }
}

/**
 @param
*/
void listener::doMakeConnection(int connfd)
{
  if (makeNewConnectionCallBack_) {
    // can not set nonblocking just return and lost this connection!
    if (setNonBlock(connfd) < 0) {
      LOG(ERROR) << "set NonBlocking Error";
      close(connfd);
    }
    makeNewConnectionCallBack_(connfd, getEventBase());
  } else {
    close(connfd);
  }
}

/**
 @param
*/
int listener::createSocketAndListen()
{
  struct sockaddr_in srvaddr;
  socklen_t len = sizeof(srvaddr);
  listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd_ < 0) {
    LOG(ERROR) <<  "socket error!";
    return -1;
  }
  srvaddr.sin_family = AF_INET;
  inet_aton(host_.c_str(), &(srvaddr.sin_addr));
  srvaddr.sin_port = htons(port_);
  len = sizeof(srvaddr);
  if (bind(listenfd_, (struct sockaddr*)&srvaddr, len) < 0) {
    LOG(ERROR) << "socket bind error!";
    return -1;
  }
  if (setNonBlock(listenfd_) < 0) {
    return -1;
  }
  int ret = listen(listenfd_, 100);
  if (ret < 0) {
    LOG(ERROR) << "listen error";
    return -1;
  }
  if (setNonBlock(listenfd_) < 0) {
    return -1;
  }
  return 0;
}

/**
 @param
*/
void listener::start()
{
  if (createSocketAndListen() < 0) {
    return;
  }
  ev_  = (struct event*)malloc(sizeof(struct event));
  event_set(ev_, listenfd_, EV_READ |EV_PERSIST, listenCallBack, this);
  event_base_set(base_, ev_);
  event_add(ev_, NULL);
}

