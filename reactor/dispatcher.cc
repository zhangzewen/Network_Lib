// Copyright [2015] <Zhang Zewen>
#include "dispatcher.h"

#include <sys/epoll.h>
#include <assert.h>
#include <list>
#include <glog/logging.h>

#include "event.h"

Dispatcher::Dispatcher()
{
}

Dispatcher::Dispatcher(Poller* poller) : poller_(poller)
{
}

Dispatcher::~Dispatcher()
{
}

bool Dispatcher::addEvent(Event* ev, short what, int flag)
{
  assert(ev);
  assert(what);
  assert(flag);
  if (!poller_->addEvent(ev, what, flag)) {
    return false;
  }
  return true;
}

bool Dispatcher::delEvent(Event* ev, short what, int flag)
{
  assert(ev);
  assert(what);
  assert(flag); 
  if (!poller_->delEvent(ev, what, flag)) {
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
  while (activeEventList_.empty()) {
    Event* ev = activeEventList_.back();
    ev->handleEvent();
    activeEventList_.pop_back();
  }
}

/**
  in every loop, wher an event fired ,put it into
  the active event list, will be itered and call the 
  callback function binded the event 
*/
void Dispatcher::registActiveEvent(Event* ev)
{
  assert(NULL != ev);
  activeEventList_.push_front(ev);
  return;
}

bool Dispatcher::eventAddTimer(Event* ev, struct timeval* timeout)
{
  return true;
}
bool Dispatcher::eventDelTimer(Event* ev, struct timeval* timeout)
{
  return true;
}
