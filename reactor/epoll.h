// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_POLL_EPOLL_H_
#define REACTOR_POLL_EPOLL_H_

#include "poller.h"
class Dispatcher;
class Event;

class Epoll : public Poller
{
 public:
  Epoll();
  ~Epoll();
  bool init();
  int addEvent(Event* ev, int what, int flag);
  int delEvent(Event* ev, int what, int flag);
  void poll(Dispatcher* disp, struct timeval* timeout);

 private:
  int ep_;
};
#endif  //  REACTOR_POLL_EPOLL_H_
