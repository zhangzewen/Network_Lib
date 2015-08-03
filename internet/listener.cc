#include "listener.h"
#include "util.h"
#include "connection.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <event.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

//constructor
listener::listener(const std::string& host, int port, struct event_base* base) : host_(host),
  port_(port), base_(base)
{
}

//constructor
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
  assert(NULL == arg);
  listener* listen = static_cast<listener*>(arg);
  if (event & EV_READ) {
    //struct event* ev = static_cast<struct event*> (arg);
    struct sockaddr_in cliaddr;
    socklen_t len;
    int connfd = 0;
    if ((connfd = accept(fd, (struct sockaddr*)&cliaddr, &len)) < 0) {
      std::cerr << "Make connection error!" << std::endl;
      return;
    }
    if (!listen->doMakeConnection(connfd)) {
      std::cerr << "Make a New Connection Error" << std::endl;
    } else {
      std::cerr << "Make a New Connection OK" << std::endl;
    }

  } else {
    std::cerr << "We do not care listener write or error!" << std::endl;
  }
}

/**
 @param
*/
bool listener::doMakeConnection(int connfd)
{
  //listen->makeConnection(connfd, listen->getEventBase());
  if (makeNewConnectionCallBack_) {
    if (Util::setNonBlock(connfd) < 0) { // can not set nonblocking just return and lost this connection!
      std::cerr << "set NonBlocking Error" << std::endl;
      close(connfd);
      return false;
    }
    if (makeNewConnectionCallBack_(connfd, getEventBase())) {
      return true;
    }
  } else {
    close(connfd);
    return false;
  }
  return false;
}

int listener::createSocketAndListen()
{
  struct sockaddr_in srvaddr;
  socklen_t len = sizeof(srvaddr);
  listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd_ < 0) {
    std::cerr <<  "socket error!" << std::endl;
    return -1;
  }
  srvaddr.sin_family = AF_INET;
  //srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  inet_aton(host_.c_str(), &(srvaddr.sin_addr));
  srvaddr.sin_port = htons(port_);
  len = sizeof(srvaddr);
  if (bind(listenfd_, (struct sockaddr*)&srvaddr, len) < 0) {
    std::cerr << "socket bind error!" << std::endl;
    return -1;
  }
  if (Util::setNonBlock(listenfd_) < 0) {
    return -1;
  }
  int ret = listen(listenfd_, 100);
  if (ret < 0) {
    std::cerr << "listen error" << std::endl;
    return -1;
  }
  if (Util::setNonBlock(listenfd_) < 0) {
    return -1;
  }
  return 0;
}

void listener::start()
{
  if (createSocketAndListen() < 0) {
    return ;
  }
  ev_  = (struct event*)malloc(sizeof(struct event));
  event_set(ev_, listenfd_, EV_READ |EV_PERSIST, listenCallBack, this);
  event_base_set(base_, ev_);
  event_add(ev_, NULL);
}

