#ifndef REACTOR_POLL_POLLER_H_
#define REACTOR_POLL_POLLER_H_

class Dispatcher;
class Event;
class Poller
{
public:
    virtual void poll(Dispatcher* disp, struct timeval* timeout){
    }
    virtual int addEvent(Event* ev){
        return 0;
    }
    virtual int delEvent(Event* ev) {
        return 0;
    }
    virtual int addReadEvent(Event* ev) {
        return 0;
    }
    virtual int addWriteEvent(Event* ev) {
        return 0;
    }
    virtual int delReadEvent(Event* ev) {
        return 0;
    }
    virtual int delWriteEvent(Event* ev) {
        return 0;
    }
protected:
    Poller() {}
    ~Poller(){}
};
#endif  // REACTOR_POLL_POLLER_H_
