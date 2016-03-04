#ifndef REACTOR_POLL_POLLER_H_
#define REACTOR_POLL_POLLER_H_

#include <memory>
class Dispatcher;
class Event;
class Timer;
class Poller
{
public:
    virtual void poll(Dispatcher* disp, const Timer& timeout, int flag){
    }
    virtual int addEvent(std::shared_ptr<Event>& ev, int event){
        return 0;
    }
    virtual int delEvent(std::shared_ptr<Event>& ev, int event) {
        return 0;
    }
    virtual void clearEvent(std::shared_ptr<Event>& ev, int event) {
    }
    ~Poller(){}
protected:
    Poller() {}
};
#endif  // REACTOR_POLL_POLLER_H_
