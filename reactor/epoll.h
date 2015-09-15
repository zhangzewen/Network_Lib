#ifndef REACTOR_POLL_EPOLL_H_
#define REACTOR_POLL_EPOLL_H_

#include "poller.h"
class dispatcher;

class Epoll : public poller
{
 public:
  Epoll();
  ~Epoll(); 
  void poll(dispatcher* disp, void* arg);
 private:
  int ep_;
};
#endif  //  REACTOR_POLL_EPOLL_H_
