// Copyright [2015] <Zhang Zewen>

#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
#include <glog/logging.h>

#include "event.h"
#include "listener.h"
#include "util.h"


using namespace std::placeholders;

Listener::Listener(const std::string& host, int port,
  const std::shared_ptr<Dispatcher>& disp) : host_(host), port_(port), disp_(disp)
{
}

Listener::Listener(const std::string& host, int port) : host_(host),
  port_(port), disp_(NULL)
{
}

/**
  @param fd 
  @param event 
  @param arg an argument to be passed to the callback function
*/
void Listener::listenCallBack(std::shared_ptr<Event> ev)
{
  if (ev->isActive() && ev->isReadReady()) {
    do {
      struct sockaddr_in cliaddr;
      socklen_t len;
      int connfd = 0;
      if ((connfd = accept(listenfd_, (struct sockaddr*)&cliaddr, &len)) < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          LOG(ERROR) << "accept() not ready";
          return;
        }
        LOG(ERROR) << "accept error!";
        return;
      }
      doMakeConnection(connfd);
    }while(true);
  }
}

/**
 @param
*/
void Listener::doMakeConnection(int connfd)
{
  if (makeNewConnectionCallBack_) {
    // can not set nonblocking just return and lost this connection!
    if (setNonBlock(connfd) < 0) {
      LOG(ERROR) << "set NonBlocking Error";
      close(connfd);
    }
    makeNewConnectionCallBack_();
  } else {
    close(connfd);
  }
}

/**
 @param
*/
int Listener::createSocketAndListen()
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
void Listener::start()
{
  if (createSocketAndListen() < 0) {
    return;
  }
  if (disp_) {
      disp_->addReadEvent(listenfd_, std::bind(&Listener::listenCallBack, shared_from_this(), _1));
  }
}

