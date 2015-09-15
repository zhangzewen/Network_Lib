#ifndef REACTOR_DISPATCHER_H_
#define REACTOR_DISPATCHER_H_

#include "event.h"

class dispatcher
{
public:
  dispatcher();
  ~dispatcher();
  bool addEvent(event* ev, short what, int flag);
  bool delEvent(event* ev, short what, int flag);
  void loop();
private:
  std::list<event*> events_;
};
#endif  //  REACTOR_DISPATCHER_H_
