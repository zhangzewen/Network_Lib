// Copyright [2015] <Zhang Zewen>
#include "epoll.h"

#include <assert.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>

#include <glog/logging.h>

#include "event.h"
#include "dispatcher.h"
#include "internal-define.h"

Epoll::Epoll() : ep_(-1)
{
}

Epoll::~Epoll()
{
}

bool Epoll::init() {
  if ((ep_ = epoll_create(1024)) < 0) {
    LOG(ERROR) << "epoll_create error";
    return false;
  }
  return true;
}

void Epoll::poll(Dispatcher* disp, struct timeval* timeout) {
  assert(disp);
  assert(timeout);

  struct epoll_event firedEvents[1024] = {0, {0}};
  int nevents = epoll_wait(ep_, firedEvents, 1024, -1);
  if (nevents < 0) {
    LOG(ERROR) << "epoll_wait error!";
    return;
  }

  if (nevents == 0) {
    LOG(INFO) << "no event happend";
    return;
  }

  for (int i = 0; i < nevents; i++) {
    int revents = firedEvents[i].events;
    Event* ev = static_cast<Event*>(firedEvents[i].data.ptr);
    if (NULL == ev) {
      LOG(ERROR) << "when loop the fired events set, the fd: "
                 << firedEvents[i].data.fd << "related event is NULL!";
      continue;
    }

    if (revents & (EPOLLERR|EPOLLHUP)) {
      LOG(ERROR) << "epoll_wait error at fd: " << ev->getFd() << " ,ev: " << ev;
      continue;
    }

    if ((revents & EPOLLIN) && ev->isActive()) {  //  readable
      ev->setReady(true);
      disp->registActiveEvent(ev);
    }
    if ((revents & EPOLLOUT) && ev->isActive()) {  //  writeable
      ev->setReady(true);
      disp->registActiveEvent(ev);
    }
  }
}


int Epoll::addEvent(Event* ev, int what, int flag)
{
  assert(NULL != ev);
  assert(flag);

  int op = 0;
  struct epoll_event ee;
  int revents = ev->getRegistEvents();

  if (what & EV_READ) {
    revents |= EPOLLIN;
  }

  if (what & EV_WRITE) {
    revents |= EPOLLOUT;
  }

  if (ev->isActive()) {
    op = EPOLL_CTL_MOD;
  } else {
    op = EPOLL_CTL_ADD;
  }

  ee.events = revents;
  ee.data.ptr = static_cast<void*>(ev);
  if (epoll_ctl(ep_, op, ev->getFd(), &ee) == -1) {
    LOG(ERROR) << "epoll_ctl: op = " << op << " ev: " << ev << " Error";
    return -1;
  }
  ev->updateRegistEvents(revents);
  ev->setActive(true);
  return 0;
}

int Epoll::delEvent(Event* ev, int what, int flag)
{
  assert(NULL != ev);
  assert(flag);

  int op = 0;
  struct epoll_event ee;
  int revents = ev->getRegistEvents();
  
  if (!ev->isActive()) {
    LOG(ERROR) << "Delete ev: " << ev << "ev active: " << ev->isActive();
    return 0;
  }

  if (what & EV_READ) {
    if (revents & EPOLLIN) {
      revents &= ~EPOLLIN;
    }
  }
  if (what & EV_WRITE) {
    if (revents & EPOLLOUT) {
      revents &= ~EPOLLOUT;
    }
  }
  if (ev->isActive()) {
    ee.events = revents;
    ee.data.ptr = static_cast<void*>(ev);
    op = EPOLL_CTL_MOD;
  } else {
    ee.events = 0;
    ee.data.ptr = NULL;
    op = EPOLL_CTL_DEL;
  }

  if (epoll_ctl(ep_, op, ev->getFd(), &ee) == -1) {
    LOG(ERROR) << "epoll_ctl: op = " << op << " ev: " << ev << " Error: " << strerror(errno);
    return -1;
  }
  ev->updateRegistEvents(revents);
  if (op == EPOLL_CTL_DEL) {
    ev->setActive(false);
  }
  return 0;
}
