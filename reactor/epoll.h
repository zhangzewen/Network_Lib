// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_POLL_EPOLL_H_
#define REACTOR_POLL_EPOLL_H_

#include "poller.h"
class dispatcher;
class event;

class Epoll : public poller
{
 public:
  Epoll();
  ~Epoll();
  bool init();
  int addEvent(event* ev, int what, int flag);
  int delEvent(event* ev, int what, int flag);
  void poll(dispatcher* disp, void* arg);

 private:
  int ep_;
};
#endif  //  REACTOR_POLL_EPOLL_H_
