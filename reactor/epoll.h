// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_POLL_EPOLL_H_
#define REACTOR_POLL_EPOLL_H_

#include "poller.h"
#include <map>
#include <memory>
class Dispatcher;
class Event;

class Epoll : public Poller
{
public:
    Epoll();
    ~Epoll();
    bool init();
    virtual int addEvent(std::shared_ptr<Event>& ev);
    virtual int delEvent(std::shared_ptr<Event>& ev);
    virtual int addReadEvent(std::shared_ptr<Event>& ev);
    virtual int addWriteEvent(std::shared_ptr<Event>& ev);
    virtual int delReadEvent(std::shared_ptr<Event>& ev);
    virtual int delWriteEvent(std::shared_ptr<Event>& ev);
    virtual void poll(Dispatcher* disp, struct timeval* timeout);
private:
    std::shared_ptr<Event> getEventByFd(int fd, const std::map<int, std::shared_ptr<Event> >& events);
    std::shared_ptr<Event> getReadEventByFd(int fd);
    std::shared_ptr<Event> getWriteEventByFd(int fd);
    int epf_;
    std::map<int, std::shared_ptr<Event> > readEvents_;
    std::map<int, std::shared_ptr<Event> > writeEvents_;
    std::map<int, std::shared_ptr<Event> > timeoutEvents_;
};
#endif  //  REACTOR_POLL_EPOLL_H_
