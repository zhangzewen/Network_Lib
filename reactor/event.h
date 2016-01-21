// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_EVENT_H_
#define REACTOR_EVENT_H_

#include "reactor.h"
#include <sys/time.h>

class Event : public std::enable_shared_from_this<Event>
{
public:
    Event() : fd_(-1), ready_(false), active_(false), timeout_(false),
        registEvents_(0), privData_(NULL) {
    }
    ~Event();
    // typedef boost::function<void(Event*)> handler;
    // fd
    int getFd() const { return fd_; }
    void setFd(int fd) { fd_ = fd; }
    // ready_
    bool isReady() const {return ready_;}

    void setReady(bool ready) { ready_ = ready;}

    // active_
    bool isActive() const { return active_;}

    void setActive(bool active) { active_ = active;}

    // timeout_
    bool isTimeout() const { return timeout_;}

    void setTimeout(bool timeout) { timeout_ = timeout;}

    void setPrivData(void* privData) { privData_ = privData;}

    void* getPrivData() const { return privData_;}

    void handleEvent() {
        if (handler_) {
            handler_(shared_from_this());
        }
    }

    void setEventHandler(const eventHandler& cb) {
        handler_ = cb;
    }

    int getRegistEvents() const {
        return registEvents_;
    }

    void enableReadEvent() {
        registEvents_ |= REACTOR_EV_READ;
    }

    void disableReadEvent() {
        registEvents_ &= ~REACTOR_EV_READ;
    }

    void enableWriteEvent() {
        registEvents_ |= REACTOR_EV_WRITE;
    }

    void disableWriteEvent() {
        registEvents_ &= ~REACTOR_EV_WRITE;
    }

    void enableTimeoutEvent() {
        registEvents_ |= REACTOR_EV_TIMEOUT;
    }

    void disableTimeoutEvent() {
        registEvents_ &= ~REACTOR_EV_WRITE;
    }


    void setDispatcher(const std::shared_ptr<Dispatcher>& disp) {
        dispatcher_ = disp;
    }

    std::shared_ptr<Dispatcher> getDispatcher() const {
        return dispatcher_;
    }

private:
    int fd_;
    // whether event is ready for reading or writing
    bool ready_;
    // whether  event is registed
    bool active_;
    // whether event for reading or writing is timeout
    bool timeout_;
    int registEvents_;
    void* privData_;
    eventHandler handler_;
    Timer timer_;
    std::shared_ptr<Dispatcher> dispatcher_;
};
#endif  //  REACTOR_EVENT_H_
