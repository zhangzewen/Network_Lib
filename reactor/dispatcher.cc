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

bool Dispatcher::addEvent(std::shared_ptr<Event>& ev)
{
    assert(ev);
    if (0 != poller_->addEvent(ev)) {
        return false;
    }
    return true;
}


bool Dispatcher::delReadEvent(std::shared_ptr<Event>& ev)
{
    ev->disableReadEvent();
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

bool Dispatcher::addReadEvent(int fd, const eventHandler& readEventHandler)
{
    std::shared_ptr<Event> ev(new Event());
    ev->setFd(fd);
    ev->setDispatcher(shared_from_this());
    ev->setEventHandler(readEventHandler);
    if (0 != poller_->addReadEvent(ev)) {
        return false;
    }
    return true;
}

bool Dispatcher::addWriteEvent(int fd, const eventHandler& readEventHandler)
{
    std::shared_ptr<Event> ev(new Event());
    ev->setFd(fd);
    ev->setEventHandler(readEventHandler);
    if (0 != poller_->addWriteEvent(ev)) {
        return false;
    }
    return true;
}

void Dispatcher::loop()
{
    poller_->poll(this, NULL);
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
