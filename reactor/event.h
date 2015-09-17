// Copyright [2015] <Zhang Zewen>
#ifndef REACTOR_EVENT_H_
#define REACTOR_EVENT_H_

#include <boost/function.hpp>

class Event
{
 public:
  Event();
  ~Event();
  typedef boost::function<void(Event*)> handler;
  int getFd() const {
    return fd_;
  }
  void setFd(int fd) {
    fd_ = fd;
  }
  bool isReady() const {
    return ready_;
  }
  void setReady(bool ready) {
    ready_ = ready;
  }
  bool isActive() const {
    return active_;
  }
  void setActive(bool active) {
    active_ = active;
  }
  bool isTimeout() const {
    return timeout_;
  }
  void setTimeout(bool timeout) {
    timeout_ = timeout;
  }
  bool isTimerSet() const {
    return timerSet_;
  }
  void setTimerSet(bool timerset) {
    timerSet_ = timerset;
  }
  void setPrivData(void* privData) {
    privData_ = privData;
  }
  void* getPrivData() const {
    return privData_;
  }
  void handleEvent() {
    if (handler_) {
      handler_(this);
    }
  }
  void setEventHandler(const handler& cb) {
    handler_ = cb;
  }
  int getRegistEvents() const {
    return registEvents_;
  }

  void updateRegistEvents(int events) {
    registEvents_ = events;
  }

 private:
  int fd_;
  bool ready_;
  bool active_;
  bool timeout_;
  bool timerSet_;
  int registEvents_;
  void* privData_;
  handler handler_;
};
#endif  //  REACTOR_EVENT_H_
