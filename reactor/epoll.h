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
    int addEvent(std::shared_ptr<Event>& ev);
    int delEvent(std::shared_ptr<Event>& ev);
    int addReadEvent(std::shared_ptr<Event>& ev);
    int addWriteEvent(std::shared_ptr<Event>& ev);
    int delReadEvent(std::shared_ptr<Event>& ev);
    int delWriteEvent(std::shared_ptr<Event>& ev);
    void poll(Dispatcher* disp, struct timeval* timeout);
private:
    Event* getEventByFd(int fd, const std::map<int, std::shared_ptr<Event> >& events);
    Event* getReadEventByFd(int fd);
    Event* getWriteEventByFd(int fd);
    int epf_;
    std::map<int, std::shared_ptr<Event> > readEvents_;
    std::map<int, std::shared_ptr<Event> > writeEvents_;
};
#endif  //  REACTOR_POLL_EPOLL_H_
