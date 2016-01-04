#ifndef REACTOR_POLL_POLLER_H_
#define REACTOR_POLL_POLLER_H_

class Dispatcher;
class Event;
class Poller
{
public:
    virtual void poll(Dispatcher* disp, struct timeval* timeout);
    virtual int addEvent(Event*, int what, int flag);
    virtual int delEvent(Event*, int what, int flag);
    virtual int addEvent(Event* ev);
    virtual int delEvent(Event* ev);
protected:
    Poller() {}
    ~Poller(){}
};
#endif  // REACTOR_POLL_POLLER_H_
