// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_DISPATCHER_H_
#define REACTOR_DISPATCHER_H_

#include "event.h"
#include "poller.h"

#include <list>
#include <memory>

class Dispatcher
{
public:
    Dispatcher();
    explicit Dispatcher(Poller* poller);
    ~Dispatcher();
    void setPoller(Poller* poller) {
        poller_ = poller;
    }
    const Poller* getPoller() const {
        return poller_;
    }
    bool addReadEvent(int fd, const Event::handler& readEventHandler);
    bool addWriteEvent(int fd, const Event::handler& readEventHandler);
    bool addEvent(Event* ev);
    bool delEvent(Event* ev);
    bool eventAddTimer(Event* ev, struct timeval* timeout);
    bool eventDelTimer(Event* ev, struct timeval* timeout);
    void processActiveEvents();
    void addActiveEvent(Event* ev);
    void loop();

private:
    Poller* poller_;
    std::list<std::shared_ptr<Event> > activeEventList_;
};
#endif  //  REACTOR_DISPATCHER_H_
