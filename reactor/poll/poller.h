#ifndef REACTOR_POLL_POLLER_H_
#define REACTOR_POLL_POLLER_H_

class dispatcher;
class poller
{
public:
  virtual void poll(dispatcher* disp, void* arg){
    assert(disp);
    assert(arg);
  } 
protected:
  poller() {}
};
#endif  // REACTOR_POLL_POLLER_H_
