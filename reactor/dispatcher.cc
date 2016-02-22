// Copyright [2015] <Zhang Zewen>
#include "dispatcher.h"
#include <sys/epoll.h>
#include <glog/logging.h>

#include <assert.h>
#include <list>

Dispatcher::Dispatcher()
{
}

Dispatcher::Dispatcher(Poller* poller) : poller_(poller)
{
}

Dispatcher::~Dispatcher()
{
}

bool Dispatcher::addEvent(std::shared_ptr<Event>& ev, int timeout)
{
    assert(ev);

    if (timeout > 0) {
        Timer time;
        time.init(timeout);
        timeout_.insert(time, ev);
    }
    if (0 != poller_->addWriteEvent(ev)) {
        return false;
    }
    return true;
}


bool Dispatcher::delReadEvent(std::shared_ptr<Event>& ev)
{
    ev->disableReadEvent();
    return delEvent(ev);

}

bool Dispatcher::delWriteEvent(std::shared_ptr<Event>& ev)
{
    ev->disableWriteEvent();
    return delEvent(ev);
}

bool Dispatcher::delEvent(std::shared_ptr<Event>& ev)
{
    assert(ev);
    if (0 != poller_->delEvent(ev)) {
        return false;
    }
    return true;
}

bool Dispatcher::disableReadEvent(std::shared_ptr<Event>& ev)
{
    ev->disableReadEvent();
    return delEvent(ev);
}

bool Dispatcher::disableWriteEvent(std::shared_ptr<Event>& ev)
{
    ev->disableWriteEvent();
    return delEvent(ev);
}

bool Dispatcher::disableTiemoutEvent(std::shared_ptr<Event>& ev)
{
    return true;
}

bool Dispatcher::addReadEvent(int fd, const eventHandler& readEventHandler, int timeout)
{
    std::shared_ptr<Event> ev(new Event());
    ev->setFd(fd);
    ev->setDispatcher(shared_from_this());
    ev->setEventHandler(readEventHandler);
    if (timeout > 0) {  // now we set timeout
        Timer time;
        time.init(timeout);
        timeout_.insert(time, ev);
    }

    if (0 != poller_->addReadEvent(ev)) {
        return false;
    }
    return true;
}

bool Dispatcher::addWriteEvent(int fd, const eventHandler& readEventHandler, int timeout)
{
    std::shared_ptr<Event> ev(new Event());
    ev->setFd(fd);
    ev->setEventHandler(readEventHandler);
    if (timeout > 0) {
        Timer time;
        time.init(timeout);
        timeout_.insert(time, ev);
    }
    if (0 != poller_->addWriteEvent(ev)) {
        return false;
    }
    return true;
}

void Dispatcher::loop()
{
    poller_->poll(this, nextTimeout());
    // get timeout event to active list
    //
    Timer now;
    now.now();
    std::shared_ptr<Event> ev;
    while(ev = getLatestEvent()) {
        if (now <= ev->getTimeout()) {
            ev->setTimeout(true);
            addActiveEvent(ev);
            timeout_.Delete(ev->getTimeout());
        }
    }
    processActiveEvents();
}

/**
  process the active events
  */
void Dispatcher::processActiveEvents()
{
    while (!activeEventList_.empty()) {
        std::shared_ptr<Event> ev = activeEventList_.back();
        ev->handleEvent();
        activeEventList_.pop_back();
    }
}

/**
  in every loop, wher an event fired ,put it into
  the active event list, will be itered and call the
  callback function binded the event
  */
void Dispatcher::addActiveEvent(std::shared_ptr<Event>& ev)
{
    assert(NULL != ev);
    activeEventList_.push_front(ev);
    return;
}

bool Dispatcher::eventAddTimer(Event* ev, struct timeval* timeout)
{
    assert(NULL == timeout);
    assert(ev == NULL);
    return true;
}

bool Dispatcher::eventDelTimer(Event* ev, struct timeval* timeout)
{
    assert(NULL == timeout);
    assert(ev == NULL);
    return true;
}

int Dispatcher::nextTimeout()
{
    std::shared_ptr<Event> ev = getLatestEvent();
    Timer now;
    Timer timeout = now - ev->getTimeout();
    return timeout.convertToMilliseconds();
}

std::shared_ptr<Event> Dispatcher::getLatestEvent()
{
    RBTree<Timer, std::shared_ptr<Event> >::Node* node =  timeout_.minimum();
    return node->value;
}

bool Dispatcher::addTimer(std::shared_ptr<Event>&, int timeout)
{


    return true;
}

bool Dispatcher::delTimer(std::shared_ptr<Event>&)
{
    return true;
}
