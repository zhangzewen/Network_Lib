#include <sys/epoll.h>
#include <list>
#include "dispatcher.h"
#include <assert.h>
#include <glog/logging.h>

#include "event.h"

dispatcher::dispatcher()
{
}

dispatcher::dispatcher(poller* poller) : poller_(poller)
{
}

dispatcher::~dispatcher()
{
}

bool dispatcher::addEvent(event* ev, short what, int flag)
{
  assert(ev);
  assert(what);
  assert(flag); 
  return true;
}

bool dispatcher::delEvent(event* ev, short what, int flag)
{
  assert(ev);
  assert(what);
  assert(flag); 
  return true;
}

void dispatcher::loop()
{
  //  struct epoll_event firedEvents[1024] = {0, {0}};
  //  while(1) {
  //    int nfds = epoll_wait(epollfd_, firedEvents, 1024, -1);
  //    if (nfds == -1) {
  //      LOG(ERR) << "epoll_wait error!";
  //      return;
  //    }   
  //    for (int i = 0; i < nfds; ++i) {
  //      int activefd = events_[i].data.fd;
  //      if (channels_.find(activefd) == channels_.end()) {
  //        continue;
  //      }   
  //      std::shared_ptr<Channel> channel = channels_[activefd];
  //      channel->handleEvent(events_[i].events);
  //    }   
  //  }
  //
  //  count time
  
  poller_->poll(this, NULL);
  processActiveEvents();
}

void dispatcher::processActiveEvents()
{
  while(activeEventList_.empty()) {
    event* ev = activeEventList_.back();
    ev->handleEvent();
    activeEventList_.pop_back();
  }
}

void dispatcher::registActiveEvent(event* ev)
{
  assert(NULL != ev);
  activeEventList_.push_front(ev);
}
