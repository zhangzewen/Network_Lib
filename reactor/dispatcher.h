// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_DISPATCHER_H_
#define REACTOR_DISPATCHER_H_

#include "reactor.h"

#include <list>
#include <memory>

class Dispatcher
{
public:
    Dispatcher();
    explicit Dispatcher(Poller* poller);
    ~Dispatcher();
    void setPoller(const std::shared_ptr<Poller>&  poller) {
        poller_ = poller;
    }
    const Poller* getPoller() const {
        return poller_.get();
    }
    bool addReadEvent(int fd, const eventHandler& readEventHandler);
    bool addWriteEvent(int fd, const eventHandler& writeEventHandler);
    bool addEvent(std::shared_ptr<Event> ev);
    bool delEvent(std::shared_ptr<Event> ev);
    bool eventAddTimer(Event* ev, struct timeval* timeout);
    bool eventDelTimer(Event* ev, struct timeval* timeout);
    void processActiveEvents();
    void addActiveEvent(Event* ev);
    void loop();

private:
    std::shared_ptr<Poller> poller_;
    std::list<std::shared_ptr<Event> > activeEventList_;
};
#endif  //  REACTOR_DISPATCHER_H_
