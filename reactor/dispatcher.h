// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_DISPATCHER_H_
#define REACTOR_DISPATCHER_H_

//#include "reactor.h"

#include <list>
#include <memory>
#include <string>

#include "RBTree.h"
#include "timer.h"
#include "callBackHandle.h"
#include "poller.h"

class Event;

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
    bool addReadEvent(int fd,
        const eventHandler& readEventHandler, int timeout = 0);
    bool addWriteEvent(int fd,
        const eventHandler& writeEventHandler, int timeout = 0);
    bool addTimer(std::shared_ptr<Event>&, int timeout);
    bool delTimer(std::shared_ptr<Event>&);
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
    void runAt(const std::string& name,
            int time, const eventHandler& timeoutEventHandler);
    void runEvery(const std::string& name,
            int time, const eventHandler& timeoutEventHandler);
    Timer nextTimeout(int& flag);
    std::shared_ptr<Event> getLatestEvent();
    void loop();

 private:
    struct TimerCmp {
        bool operator() (Timer& tm1, Timer& tm2) {
            return tm1 < tm2 ? true : false;
        }
    };
    void runEveryCallBack(std::shared_ptr<Event> ev,
            const eventHandler& timeoutEventHandler,
            const std::string& name, int time) {
        timeoutEventHandler(ev);
        runEvery(name, time, timeoutEventHandler);
    }
    std::shared_ptr<Poller> poller_;
    std::list<std::shared_ptr<Event> > activeEventList_;
    RBTree<Timer, std::shared_ptr<Event>, TimerCmp> timeout_;
    bool addTimeoutEvent(const std::string& name,
            int timeout, const eventHandler& timeoutEventHandler);
};
#endif  //  REACTOR_DISPATCHER_H_
