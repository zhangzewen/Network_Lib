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


bool Dispatcher::delReadEvent(std::shared_ptr<Event>& ev)
{
    poller_->delEvent(ev, REACTOR_EV_READ);
    poller_->clearEvent(ev, REACTOR_EV_READ);

    return true;
}

bool Dispatcher::delWriteEvent(std::shared_ptr<Event>& ev)
{
    poller_->delEvent(ev, REACTOR_EV_WRITE);
    poller_->clearEvent(ev, REACTOR_EV_WRITE);
    return true;
}

bool Dispatcher::delEvent(std::shared_ptr<Event>& ev)
{
    poller_->delEvent(ev, REACTOR_EV_READ | REACTOR_EV_WRITE);
    poller_->clearEvent(ev, REACTOR_EV_READ | REACTOR_EV_WRITE);
    return true;
}


bool Dispatcher::disableReadEvent(std::shared_ptr<Event>& ev)
{
    return poller_->delEvent(ev, REACTOR_EV_READ);

}

bool Dispatcher::disableWriteEvent(std::shared_ptr<Event>& ev)
{
    return poller_->delEvent(ev, REACTOR_EV_WRITE);
}

bool Dispatcher::disableTiemoutEvent(std::shared_ptr<Event>& ev)
{
    return true;
}

bool Dispatcher::addReadEvent(int fd,
    const eventHandler& readEventHandler, int timeout)
{
    std::shared_ptr<Event> ev(new Event());
    ev->setFd(fd);
    ev->setDispatcher(shared_from_this());
    ev->setEventHandler(readEventHandler);
    if (timeout > 0) {  // now we set timeout
        Timer time(timeout);
        Timer now;
        timeout_.insert(time + now, ev);
    }

    if (0 != poller_->addEvent(ev, REACTOR_EV_READ)) {
        return false;
    }
    return true;
}

bool Dispatcher::addWriteEvent(int fd,
    const eventHandler& readEventHandler, int timeout)
{
    std::shared_ptr<Event> ev(new Event());
    ev->setFd(fd);
    ev->setEventHandler(readEventHandler);
    if (timeout > 0) {
        Timer time(timeout);
        Timer now;
        timeout_.insert(time + now, ev);
    }
    if (0 != poller_->addEvent(ev, REACTOR_EV_WRITE)) {
        return false;
    }
    return true;
}

void Dispatcher::loop()
{
    while (true) {
        Timer timeout;
        int flag = POLL_TIMEOUT_WAIT_TYPE;
        //if there are activeEvent already, just make poller polling without wait
        if (!activeEventList_.empty()) {
            timeout.timer_reset();
            flag = POLL_NOT_WAIT_TYPE;
        } else {
            timeout = nextTimeout(flag);
        }
        poller_->poll(this, timeout, flag);
        // get timeout event to active list
        //
        Timer now;
        std::shared_ptr<Event> ev;
        while(ev = getLatestEvent()) {
            Timer timeout = ev->getTimeout();
            if (now >= timeout) {
                ev->setTimeout(true);
                addActiveEvent(ev);
                ev->setReady(true);
                timeout_.Delete(timeout);
            }
        }
        processActiveEvents();
    }
}

/**
  process the active events
  */
void Dispatcher::processActiveEvents()
{
    while (!activeEventList_.empty()) {
        std::shared_ptr<Event> ev = activeEventList_.back();
        //当事件是已经注册，并且事件触发了，才会执行回调函数
        //任务执行完成后，立马从队列尾部吧事件清除掉
        if (ev->isActive() && ev->isReady()) {
            ev->handleEvent();
        }
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

Timer Dispatcher::nextTimeout(int& flag)
{
    std::shared_ptr<Event> ev = getLatestEvent();
    Timer now;
    if (!ev) {
        now.timer_reset();
        flag = POLL_INDEFINITELY_WAIT_TYPE;
        return now;
    }
    Timer timeout = ev->getTimeout();
    if (now > timeout) {
        now.timer_reset();
        return now;
    }
    return timeout - now;
    //Timer timeout = now - ev->getTimeout();
    //return timeout.convertToMilliseconds();
}

std::shared_ptr<Event> Dispatcher::getLatestEvent()
{
    RBTree<Timer, std::shared_ptr<Event>, TimerCmp>::Node* node =  timeout_.minimum();
    if (node) {
        return node->value;
    }
    return NULL;
}

bool Dispatcher::addTimer(std::shared_ptr<Event>&, int timeout)
{


    return true;
}

bool Dispatcher::delTimer(std::shared_ptr<Event>&)
{
    return true;
}
void Dispatcher::runAt(const std::string& name, int time, const eventHandler& timeoutEventHandler)
{
    addTimeoutEvent(name, time, timeoutEventHandler);
}

bool Dispatcher::addTimeoutEvent(const std::string& name, int timeout, const eventHandler& timeoutEventHandler)
{
    if (timeout < 0) {
        return false;
    }

    std::shared_ptr<Event> ev(new Event(name));
    ev->setDispatcher(shared_from_this());
    ev->setEventHandler(timeoutEventHandler);
    Timer time(timeout);
    Timer now;
    Timer click = (time + now);
    ev->setEventTimeout(click);

    //if (0 != poller_->addEvent(ev, 0)) {
    //    return false;
    //}

    timeout_.insert(click, ev);
    ev->setActive(true);
    return true;
}
