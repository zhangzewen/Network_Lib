// Copyright [2015] <Zhang Zewen>
#include "epoll.h"

#include <assert.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>

#include <glog/logging.h>

#include "event.h"
#include "dispatcher.h"

Epoll::Epoll() : epf_(-1)
{
}

Epoll::~Epoll()
{
}

bool Epoll::init() {
    if ((epf_ = epoll_create(1024)) < 0) {
        LOG(ERROR) << "epoll_create error";
        return false;
    }
    return true;
}

void Epoll::poll(Dispatcher* disp, const Timer& timeout, int flag) {
    assert(disp);

    struct epoll_event firedEvents[1024] = {0, {0}};
    //timeout = (timeout == 0 ? -1 : timeout);
    int wait = (flag == POLL_INDEFINITELY_WAIT_TYPE ? -1 : timeout.convertToMilliseconds());
    int nevents = epoll_wait(epf_, firedEvents, 1024, wait);
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
        //Event* ev = static_cast<Event*>(firedEvents[i].data.ptr);
        int fd = firedEvents[i].data.fd;
        std::shared_ptr<Event> ev;

        if (revents & (EPOLLERR|EPOLLHUP)) {
            LOG(ERROR) << "epoll_wait error at fd: " << ev->getFd() << " ,ev: " << ev;
            continue;
        }

        if ((revents & EPOLLIN)
            && (ev = readEvents_[fd])
            && ev->isActive()) {  //  readable
            ev->setReady(true);
            disp->addActiveEvent(ev);
        }
        if ((revents & EPOLLOUT)
            && (ev = writeEvents_[fd])
            && ev->isActive()) {  //  writeable
            ev->setReady(true);
            disp->addActiveEvent(ev);
        }
    }
}

int Epoll::addEvent(std::shared_ptr<Event>& ev, int event)
{
    int op = 0;
    struct epoll_event ee;
    int events = 0;
    int prev = 0;
    int fd = ev->getFd();
    std::shared_ptr<Event> e;

    if (event & REACTOR_EV_READ) {
        events |= EPOLLIN;
        prev = EPOLLOUT;
        e = getWriteEventByFd(fd);
    }

    if (event & REACTOR_EV_WRITE) {
        events |= EPOLLOUT;
        prev = EPOLLIN;
        e = getReadEventByFd(fd);
    }

    if (e && e->isActive()) {
        op = EPOLL_CTL_MOD;
        events |= prev;
    } else {
        op = EPOLL_CTL_ADD;
    }

    ee.events = events;
    ee.data.ptr = NULL;
    ee.data.fd = fd;
    if (epoll_ctl(epf_, op, fd, &ee) == -1) {
        LOG(ERROR) << "epoll_ctl: op = " << op << " ev: " << ev << " Error";
        return -1;
    }
    if ((events & REACTOR_EV_READ) && op == EPOLL_CTL_ADD) {
        readEvents_[fd] = ev;
    }
    if ((events & REACTOR_EV_WRITE) && op == EPOLL_CTL_ADD) {
        writeEvents_[fd] = ev;
    }

    if (event & REACTOR_EV_READ) {
        ev->enableReadEvent();
    }
    if (event & REACTOR_EV_WRITE) {
        ev->enableWriteEvent();
    }

    ev->setActive(true);
    return 0;

}

int Epoll::delEvent(std::shared_ptr<Event>& ev, int event)
{
    assert(NULL != ev);
    int op = 0;
    struct epoll_event ee;
    int fd = ev->getFd();
    int events = 0;

    std::shared_ptr<Event> e;

    if (!ev->isActive()) {
        LOG(ERROR) << "Delete ev: " << ev << "ev active: " << ev->isActive();
        return 0;
    }

    if (event & REACTOR_EV_READ) {
        events = EPOLLOUT;
        e = getWriteEventByFd(fd);
    }

    if (event & REACTOR_EV_WRITE) {
        events = EPOLLIN;
        e = getReadEventByFd(fd);
    }

    if (e && e->isActive()) {
        op = EPOLL_CTL_MOD;
        ee.events = events;
        ee.data.ptr = NULL;
    } else {
        op = EPOLL_CTL_DEL;
        ee.events = 0;
        ee.data.ptr = NULL;
    }

    if (epoll_ctl(epf_, op, ev->getFd(), &ee) == -1) {
        LOG(ERROR) << "epoll_ctl: op = " << op << " ev: " << ev << " Error: " << strerror(errno);
        return -1;
    }

    ev->setActive(false);

    if (event & REACTOR_EV_READ) {
        ev->disableReadEvent();
    }

    if (event & REACTOR_EV_WRITE) {
        ev->disableWriteEvent();
    }
    return 0;
}



std::shared_ptr<Event> Epoll::getEventByFd(int fd, const std::map<int, std::shared_ptr<Event> >& events)
{
    if ( fd <= 0) {
        return NULL;
    }
    std::map<int, std::shared_ptr<Event> >::const_iterator iter = events.find(fd);
    if (iter != events.end()){
        return iter->second;
    }
    return NULL;
}

std::shared_ptr<Event> Epoll::getReadEventByFd(int fd)
{
    return getEventByFd(fd, readEvents_);
}

std::shared_ptr<Event> Epoll::getWriteEventByFd(int fd)
{
    return getEventByFd(fd, writeEvents_);
}

//int Epoll::addReadEvent(std::shared_ptr<Event>& ev)
//{
//    ev->enableReadEvent();
//    return addEvent(ev);
//}
//
//int Epoll::addWriteEvent(std::shared_ptr<Event>& ev)
//{
//    ev->enableWriteEvent();
//    return addEvent(ev);
//}
//
//int Epoll::delReadEvent(std::shared_ptr<Event>& ev)
//{
//    ev->disableReadEvent();
//    return delEvent(ev);
//}
//
//int Epoll::delWriteEvent(std::shared_ptr<Event>& ev)
//{
//    ev->disableWriteEvent();
//    return delEvent(ev);
//}
void Epoll::clearEvent(std::shared_ptr<Event>& ev, int event)
{
    if (event & REACTOR_EV_READ) {
        doClearEvent(ev, readEvents_);
    }
    if (event & REACTOR_EV_WRITE) {
        doClearEvent(ev, writeEvents_);
    }
}

void Epoll::doClearEvent(std::shared_ptr<Event>& ev, std::map<int, std::shared_ptr<Event> >& events)
{
    std::map<int, std::shared_ptr<Event> >::iterator iter;
    iter = events.find(ev->getFd());
    if (events.end() != iter) {
        events.erase(iter);
    }
}
