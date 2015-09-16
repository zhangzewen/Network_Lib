#include <assert.h>
#include <sys/epoll.h>
#include <glog/logging.h>
#include "epoll.h"
#include "event.h"
#include "dispatcher.h"
#include "internal-define.h"

Epoll::Epoll()
{
}

Epoll::~Epoll()
{
}

void Epoll::poll(dispatcher* disp, void* arg) {
  assert(disp);
  assert(arg);
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
    event* ev = static_cast<event*>(firedEvents[i].data.ptr);

    if (revents & (EPOLLERR|EPOLLHUP)) {
      LOG(ERROR) << "epoll_wait error at fd: " << ev->getFd() << " ,ev: " << ev;
    }

    if ((revents & EPOLLIN) && ev->isActive()) { //  readable
      ev->setReady(true);
      disp->registActiveEvent(ev);
    }
    if ((revents & EPOLLOUT) && ev->isActive()) { //  writeable
      ev->setReady(true);
      disp->registActiveEvent(ev);
    }
  }
}


int Epoll::addEvent(event* ev, int what, int flag)
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
  ev->setRegistEvents(revents);
  return 0;
}

int Epoll::delEvent(event* ev, int what, int flag)
{
  assert(NULL != ev);
  assert(flag);
  
  int op = 0;
  struct epoll_event ee;
  int revents = ev->getRegistEvents();
  
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
  } else {
    ee.events = 0;
    ee.data.ptr = NULL;
    op = EPOLL_CTL_DEL;
  }

  if (epoll_ctl(ep_, op, ev->getFd(), &ee) == -1) {
    LOG(ERROR) << "epoll_ctl: op = " << op << " ev: " << ev << " Error";
    return -1;
  }
  ev->setRegistEvents(revents);
  return 0;
}
