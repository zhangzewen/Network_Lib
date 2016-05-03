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
    // this will be called at CON_CONNECTED, when the client/server recv ACK + SYN successfully, means
    // connecting is already done, and accept already return a fd > 0.now the frame work will do some work
    // create a connection tie to the fd, set default callbacks for each phase that will make sure the framwork
    // run will without customize callbacks. if you want change the default logic, you shoud regist your own
    // callbacks in this callback
    typedef std::function<void (Connection*)> onEstablishedCallBack;
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
    void setOnEstablishedCallBack(const onEstablishedCallBack& cb) {
        makeNewConnectionCallBack_ = cb;
    }
 private:
    void listenCallBack(std::shared_ptr<Event> ev);
    int createSocketAndListen();
    void doMakeConnection(int connfd);
    int listenfd_;
    std::string host_;
    int port_;
    std::shared_ptr<Dispatcher> disp_;
    std::shared_ptr<Event> ev_;
    onEstablishedCallBack onEstablishedCallBack_;
};

#endif  // INTERNET_LISTENER_H_
