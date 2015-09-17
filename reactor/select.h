// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_SELECT_H_
#define REACTOR_SELECT_H_

#include <sys/select.h>
#include <map>
#include "poller.h"
class Dispatcher;
class Event;

class Select : public Poller
{
 public:
  Select();
  ~Select();
  bool init();
  int addEvent(Event* ev, int what, int flag);
  int delEvent(Event* ev, int what, int flag);
  void poll(Dispatcher* disp, struct timeval* timeout);

 private:
  int maxFd_;
  fd_set readSet_;
  fd_set writeSet_;
  std::map<int, Event*> readEventMap_;
  std::map<int, Event*> writeEventMap_;
};


#endif  //  REACTOR_SELECT_H_
