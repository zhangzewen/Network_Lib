// Copyright [2015] <Zhang Zewen>

#ifndef INTERNET_LISTENER_H_
#define INTERNET_LISTENER_H_

#include <string>
#include <memory>
#include <functional>
#include "connection.h"
#include "dispatcher.h"

class Listener : public std::enable_shared_from_this<Listener>
{
 public:
    typedef std::function<void ()> makeNewConnection;
    Listener(const std::string& host, int port, const std::shared_ptr<Dispatcher>& disp);
    Listener(const std::string& host, int port);
    ~Listener();
    void start();
    std::shared_ptr<Dispatcher> getDispatcher() const {
        return disp_;
    }
    void setDispatcher(const std::shared_ptr<Dispatcher>& disp) {
        disp_ = disp;
    }
    void setMakeNewConnectionCallBack(const makeNewConnection& cb) {
        makeNewConnectionCallBack_ = cb;
    }
    void doMakeConnection(int connfd);
 private:
    void listenCallBack(std::shared_ptr<Event> ev);
    int createSocketAndListen();
    int listenfd_;
    std::string host_;
    int port_;
    std::shared_ptr<Dispatcher> disp_;
    std::shared_ptr<Event> ev_;
    makeNewConnection makeNewConnectionCallBack_;
};

#endif  // INTERNET_LISTENER_H_
