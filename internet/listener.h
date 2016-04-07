// Copyright [2015] <Zhang Zewen>

#ifndef INTERNET_LISTENER_H_
#define INTERNET_LISTENER_H_

#include <string>
#include <memory>
#include <functional>
#include "connection.h"

class listener
{
 public:
  typedef std::function<void (int, struct event_base*)> makeNewConnection;
  listener(const std::string& host, int port, struct event_base* base);
  listener(const std::string& host, int port);
  ~listener();
  void start();
  struct event_base* getEventBase() const {
    return base_;
  }
  void setEventBase(struct event_base* base) {
    base_ = base;
  }
  void setMakeNewConnectionCallBack(const makeNewConnection& cb) {
    makeNewConnectionCallBack_ = cb;
  }
  void doMakeConnection(int connfd);
 private:
  static void listenCallBack(int fd, short event, void* arg);
  int createSocketAndListen();
  int listenfd_;
  std::string host_;
  int port_;
  struct event_base* base_;
  struct event* ev_;
  makeNewConnection makeNewConnectionCallBack_;
};

#endif  // INTERNET_LISTENER_H_
