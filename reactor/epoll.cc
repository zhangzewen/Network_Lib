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

void Epoll::poll(Dispatcher* disp, struct timeval* timeout) {
    assert(disp);
    assert(timeout == NULL);

    struct epoll_event firedEvents[1024] = {0, {0}};
    int nevents = epoll_wait(epf_, firedEvents, 1024, -1);
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

int Epoll::addEvent(std::shared_ptr<Event>& ev)
{
    assert(NULL != ev);

    int op = 0;
    struct epoll_event ee;
    int revents = ev->getRegistEvents();
    int events = 0;
    int fd = ev->getFd();
    std::shared_ptr<Event> write = getWriteEventByFd(fd);
    std::shared_ptr<Event> read = getReadEventByFd(fd);

    if (revents & REACTOR_EV_READ) {
        events |= EPOLLIN;
    }

    if (revents & REACTOR_EV_WRITE) {
        events |= EPOLLOUT;
    }

    op = EPOLL_CTL_ADD;

    if ((revents & REACTOR_EV_READ)
        && read && read->isActive()) {
        op = EPOLL_CTL_MOD;
    }
    if ((revents & REACTOR_EV_WRITE)
        && write && write->isActive()) {
        op = EPOLL_CTL_MOD;
    }

    ee.events = events;
    ee.data.ptr = static_cast<void*>(ev.get());
    ee.data.fd = fd;
    if (epoll_ctl(epf_, op, fd, &ee) == -1) {
        LOG(ERROR) << "epoll_ctl: op = " << op << " ev: " << ev << " Error";
        return -1;
    }
    if (revents & REACTOR_EV_READ) {
        readEvents_[fd] = ev;
    }
    if (revents & REACTOR_EV_WRITE) {
        writeEvents_[fd] = ev;
    }
    ev->setActive(true);
    return 0;
}

int Epoll::delEvent(std::shared_ptr<Event>& ev)
{
    assert(NULL != ev);
    int op = 0;
    struct epoll_event ee;
    int revents = ev->getRegistEvents();
    int fd = ev->getFd();
    int events = 0;
    int needreaddelete = 0;
    int needwritedelete = 0;

    std::shared_ptr<Event> e;
    std::shared_ptr<Event> write = getWriteEventByFd(fd);
    std::shared_ptr<Event> read = getReadEventByFd(fd);
    if (!ev->isActive()) {
        LOG(ERROR) << "Delete ev: " << ev << "ev active: " << ev->isActive();
        return 0;
    }

    if (revents & REACTOR_EV_READ) {
        events |= EPOLLIN;
    }
    if (revents & REACTOR_EV_WRITE) {
        events |= EPOLLOUT;
    }

    if ((events & (EPOLLIN|EPOLLOUT)) != (EPOLLIN|EPOLLOUT)) {
        if (events & EPOLLIN) {
            e = write;
            needwritedelete = 1;
            events = EPOLLIN;
        } else if (events & EPOLLOUT) {
            e = read;
            needreaddelete = 1;
            events = EPOLLOUT;
        } else {
            readEvents_[fd] = NULL;
            readEvents_.erase(fd);
            writeEvents_[fd] = NULL;
            writeEvents_.erase(fd);
        }
    }

    if (e && e->isActive()) {
        ee.events = events;
        ee.data.ptr = static_cast<void*>(e.get());
        op = EPOLL_CTL_MOD;
    } else {
        ee.events = 0;
        ee.data.ptr = NULL;
        op = EPOLL_CTL_DEL;
    }

    if (epoll_ctl(epf_, op, ev->getFd(), &ee) == -1) {
        LOG(ERROR) << "epoll_ctl: op = " << op << " ev: " << ev << " Error: " << strerror(errno);
        return -1;
    }

    if (needwritedelete) {
        writeEvents_[fd] = NULL;
        writeEvents_.erase(fd);
        write->setActive(false);
    }
    if (needreaddelete) {
        readEvents_[fd] = NULL;
        readEvents_.erase(fd);
        read->setActive(false);
    }

    if (revents & REACTOR_EV_READ) {
        readEvents_[fd] = ev;
    }

    if (revents & REACTOR_EV_WRITE) {
        writeEvents_[fd] = ev;
    }

    if (op == EPOLL_CTL_DEL) {
        ev->setActive(false);
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

int Epoll::addReadEvent(std::shared_ptr<Event>& ev)
{
    ev->enableReadEvent();
    return addEvent(ev);
}

int Epoll::addWriteEvent(std::shared_ptr<Event>& ev)
{
    ev->enableWriteEvent();
    return addEvent(ev);
}

int Epoll::delReadEvent(std::shared_ptr<Event>& ev)
{
    ev->disableReadEvent();
    return delEvent(ev);
}

int Epoll::delWriteEvent(std::shared_ptr<Event>& ev)
{
    ev->disableWriteEvent();
    return delEvent(ev);
}
