// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_SELECT_H_
#define REACTOR_SELECT_H_

#include <sys/select.h>
#include <map>
#include "poller.h"
class dispatcher;
class event;

class Select
{
 public:
  Select();
  ~Select();
  bool init();
  int addEvent(event* ev, int what, int flag);
  int delEvent(event* ev, int what, int flag);
  void poll(dispatcher* disp, void* arg);

 private:
  int maxFd_;
  fd_set readSet_;
  fd_set writeSet_;
  std::map<int, event*> readEventMap_;
  std::map<int, event*> writeEventMap_;
};


#endif  //  REACTOR_SELECT_H_
