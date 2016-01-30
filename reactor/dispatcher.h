// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_DISPATCHER_H_
#define REACTOR_DISPATCHER_H_

#include "reactor.h"

#include <list>
#include <memory>

class Dispatcher : public std::enable_shared_from_this<Dispatcher>
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
    bool addReadEvent(int fd, const eventHandler& readEventHandler, int timeout = 0);
    bool addWriteEvent(int fd, const eventHandler& writeEventHandler, int timeout = 0);
    bool addTimer(std::shared_ptr<Event>&, int timeout);
    bool delTimer(std::shared_ptr<Event>&);
    bool addEvent(std::shared_ptr<Event>& ev, int timeout);
    bool delEvent(std::shared_ptr<Event>& ev);
    bool delReadEvent(std::shared_ptr<Event>& ev);
    bool delWriteEvent(std::shared_ptr<Event>& ev);
    bool disableReadEvent(std::shared_ptr<Event>& ev);
    bool disableWriteEvent(std::shared_ptr<Event>& ev);
    bool disableTiemoutEvent(std::shared_ptr<Event>& ev);
    bool eventAddTimer(Event* ev, struct timeval* timeout);
    bool eventDelTimer(Event* ev, struct timeval* timeout);
    void processActiveEvents();
    void addActiveEvent(std::shared_ptr<Event>& ev);
    int nextTimeout();
    std::shared_ptr<Event> getLatestEvent();
    void loop();

private:
    std::shared_ptr<Poller> poller_;
    std::list<std::shared_ptr<Event> > activeEventList_;
    RBTree<Timer, std::shared_ptr<Event> > timeout_;
};
#endif  //  REACTOR_DISPATCHER_H_
