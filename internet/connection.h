// Copyright [2015] <Zhang Zewen>

#ifndef INTERNET_CONNECTION_H_
#define INTERNET_CONNECTION_H_

#include <map>
#include <string>
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>

struct http_parser;
struct bufferevent;
struct http_parser_settings;
class Listener;

class Connection
{
 public:
    typedef enum {
        // the default status when a connection construct,
        // or the connection class finally calls the disableConnection()
        // which makes the status changing to CON_DISCONNECTION
        CON_DISCONNECTED,
        // when do some init connection work such as requiring and init resource, set callback and so on,
        // we set the status CON_CONNECTING, it should change to CON_CONNECTED when listener call accept() sucessfully
        // should know that, the read callback must handle the accept() resutl EAGAIN, and the status still be CON_CONNECTING
        //CON_CONNECTING,
        // when connection buffer, events constructed and inited done,
        // state change to CON_CONNECTED, and ready to wait the data comming,
        // and zhe following state is CON_IDLE
        CON_CONNECTED,
        // After connected, which means is waiting data coming(aka waiting read event fired)
        // should always tie a timeout to this status,
        // which help to prevent unfriendly connection request.
        CON_IDLE,
        // when read event fired, we start to read data ,change the status to CON_READING,
        // the customize Protocol callback tie to this connection will be called during this status
        // and the customize Protocol code should not blocked.
        CON_READING,
        // when we done CON_READING, and write back response, the customize callback tie to this connection
        // will be called during this status
        CON_WRITTING,
        // when the write done(aka response done).
        // 1, want to relase the connection, during releasing resource,
        // the status is CON_DISCONNECTING,
        // 2. reinit connection ,wait for another request,
        // just change status to CON_IDLE, if the read buf len is not 0, change
        // the status to CON_READING, do CON_READING things
        CON_DISCONNECTING,
        // if in the CON_DISCONNECTING time, all relase job successfully done,
        // it will be CON_DISCONNECTED, and will be reuse
    } CONN_STATE;
    // this will be called at CON_CONNECTED, when the client/server recv ACK + SYN successfully, means
    // connecting is already done, now the frame work will do some work
    typedef std::function<void ()>onEstablishedCallBack;
    // when date is ready to read, the frame work will do the buffer things, the customize callback should
    // do the Protocol process things, the customize should tell the framework
    // by returing READ_DONE to jump into next phase,
    // or by returnig READ_AGAIN to do read loop again ,
    // or by returing READ_ERROR to do close this connection and RST connection.
    typedef std::function<void (Connection*, char*, int)> onMessageRecvCallBack;
    //
    typedef std::function<void (Connection*)>onReadTimeoutCallBack;
    //
    typedef std::function<void (Connection*)>onWriteTimeoutCallBack;
    //
    typedef std::function<void (Connection*)>onConnectionCloseCallBack;
    //
    typedef std::function<void (Connection*)>onWriteCallBack;
    Connection(int fd, struct event_base* base);
    ~Connection();
    // do some resource init, tie callbacks
    void init();
    void onMessage();
    void closeConnection();
    int doCloseConnection();
    void setKeepAlived(bool isKeepAlived);
    void setListener(Listener* Listener);
    bool isKeepAlived();
    bool reuseConnection();
    void enableRead();
    void enableWrite();
    void disableRead();
    void disableWrite();
    void disableReadWrite();
    void setPrivData(void * data);
    void* getPrivData() const;
    void setCustomizeOnMessageCallBack(const onMessageCallBack& cb) {
        customizeOnMessageCallBack_ = cb;
    }
    void setCustomizeOnConnectionCloseCallBack(
            const onConnectionCloseCallBack& cb) {
        customizeOnConnectionCloseCallBack_ = cb;
    }
    void setCustomizeOnWriteCallBack(const onWriteCallBack& cb) {
        customizeOnWriteCallBack_ = cb;
    }
    void startRead();
    void startWrite();
    void readDone();
    void writeDone();
    int tryWrite();
    void setConnState(CONN_STATE state) {
        conn_state_ = state;
    }
    int doWrite(const char* buf, int len);
    int setRemoteAddr(struct sockaddr_in* remoteAddr, socklen_t len);
    std::string getRemoteAddr() const {
        return remote_addr_;
    }
    void lingeringClose(Connection* conn, char* buf, int len);
    void forceCloseConnection();

 private:
    int connfd_;
    CONN_STATE conn_state_;
    bufferevent* buf_;
    std::shared_ptr<Dispatcher> disp_;
    bool keep_alived_;
    std::shared_ptr<Listener> listener_;
    std::string remote_addr_;
    void* privdata_;

    void handleRead();
    void handleWrite();
    void handleError(Connection*, short);
    //static void eventEmptyCallBack(bufferevent* bufev, void* data);
    //static void eventReadCallBack(bufferevent* bufev, void* data);
    //static void eventWriteCallBack(bufferevent* bufev, void* data);
    //static void eventErrorCallBack(bufferevent* bufev, short what, void* data);
    //static void eventTimeoutCallBack(bufferevent* bufev, void* data);

    //
    void onReadEventCallback(std::shared_ptr<Event> ev);
    void onWriteEventCallback(std::shared_ptr<Event> ev);
    onMessageCallBack customizeOnMessageCallBack_;
    onConnectionCloseCallBack customizeOnConnectionCloseCallBack_;
    onWriteCallBack customizeOnWriteCallBack_;
    bool resetEvBuffer(struct evbuffer* buf);
    bool reuseBufferEvent(struct bufferevent* bufev);
    short bufferevent_get_enabled(struct bufferevent* bufev);
};
#endif  // INTERNET_CONNECTION_H_
