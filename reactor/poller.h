#ifndef REACTOR_POLL_POLLER_H_
#define REACTOR_POLL_POLLER_H_

class dispatcher;
class event;
class poller
{
public:
  virtual void poll(dispatcher* disp, void* arg);
  virtual int addEvent(event*, int what, int flag);
  virtual int delEvent(event*, int what, int flag);
protected:
  poller() {}
  ~poller(){}
};
#endif  // REACTOR_POLL_POLLER_H_
