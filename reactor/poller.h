#ifndef REACTOR_POLL_POLLER_H_
#define REACTOR_POLL_POLLER_H_

#include <memory>
class Dispatcher;
class Event;
class Poller
{
public:
    virtual void poll(Dispatcher* disp, int timeout){
    }
    virtual int addEvent(std::shared_ptr<Event>& ev){
        return 0;
    }
    virtual int delEvent(std::shared_ptr<Event>& ev) {
        return 0;
    }
    virtual int addReadEvent(std::shared_ptr<Event>& ev) {
        return 0;
    }
    virtual int addWriteEvent(std::shared_ptr<Event>& ev) {
        return 0;
    }
    virtual int delReadEvent(std::shared_ptr<Event>& ev) {
        return 0;
    }
    virtual int delWriteEvent(std::shared_ptr<Event>& ev) {
        return 0;
    }
    ~Poller(){}
protected:
    Poller() {}
};
#endif  // REACTOR_POLL_POLLER_H_
