// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_DISPATCHER_H_
#define REACTOR_DISPATCHER_H_

#include <list>
#include "event.h"
#include "poller.h"

class dispatcher
{
 public:
  dispatcher();
  explicit dispatcher(poller* poller);
  ~dispatcher();
  void setPoller(poller* poller) {
    poller_ = poller;
  }
  const poller* getPoller() const {
    return poller_;
  }
  bool addEvent(event* ev, short what, int flag);
  bool delEvent(event* ev, short what, int flag);
  void processActiveEvents();
  void registActiveEvent(event* ev);
  void loop();

 private:
  poller* poller_;
  std::list<event*> activeEventList_;
};
#endif  //  REACTOR_DISPATCHER_H_
