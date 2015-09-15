#ifndef REACTOR_POLL_EPOLL_H_
#define REACTOR_POLL_EPOLL_H_

class Epoll : public poller
{
public:
  Epoll();
  ~Epoll(); 
  void poll(dispatcher* disp, void* arg) {
    assert(disp);
    assert(arg);
    struct epoll_event firedEvents[1024] = {0, {0}};
    int nevents = epoll_wait(ep_, firedEvents, 1024, -1);
    for (int i = 0; i < nevents; i++) {
      int revents = firedEvents[i].events;
      event* ev = static_cast<event*>(firedEvents[i].data.ptr);
      
      if ((revents & EPOLLIN) && ev->isActive()) { //  readable
        ev->setReady(true);
        disp->registEvent(ev);
      }
      if ((revents & EPOLLOUT) && ev->isActive()) { //  writeable
        ev->setReady(true);
        disp->registEvent(ev);
      }
    }
  }
private:
  int ep_;
};
#endif  //  REACTOR_POLL_EPOLL_H_
