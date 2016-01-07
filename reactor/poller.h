#ifndef REACTOR_POLL_POLLER_H_
#define REACTOR_POLL_POLLER_H_

class Dispatcher;
class Event;
class Poller
{
public:
    virtual void poll(Dispatcher* disp, struct timeval* timeout);
    virtual int addEvent(Event* ev);
    virtual int delEvent(Event* ev);
    virtual int addReadEvent(Event* ev);
    virtual int addWriteEvent(Event* ev);
    virtual int delReadEvent(Event* ev);
    virtual int delWriteEvent(Event* ev);
protected:
    Poller() {}
    ~Poller(){}
};
#endif  // REACTOR_POLL_POLLER_H_
