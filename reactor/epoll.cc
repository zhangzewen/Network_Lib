#include <assert.h>
#include <sys/epoll.h>
#include "epoll.h"
#include "event.h"
#include "dispatcher.h"

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
  for (int i = 0; i < nevents; i++) {
    int revents = firedEvents[i].events;
    event* ev = static_cast<event*>(firedEvents[i].data.ptr);

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
