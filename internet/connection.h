// Copyright [2015] <Zhang Zewen>

#ifndef INTERNET_CONNECTION_H_
#define INTERNET_CONNECTION_H_

#include <map>
#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>
struct http_parser;
struct bufferevent;
struct http_parser_settings;
struct event_base;
class listener;

class connection
{
 public:
  typedef enum {
    // when construct CLASS connection, the state initialized to CON_CONNECTING
    CON_CONNECTING,
    // when connection buffer, events constructed and inited done,
    // state change to CON_CONNECTED, and ready to wait the data comming,
    // and zhe following state is CON_IDLE
    CON_CONNECTED,
    CON_DISCONNECTING,
    CON_DISCONNECTED,
    // After connected, this state is between registed Read Event
    // and a read event fired state, which means is waiting data coming
    CON_IDLE,
    CON_READING,
    CON_WRITTING,
  } CONN_STATE;
  typedef boost::function<void (connection*, char*, int)> onMessageCallBack;
  typedef boost::function<void (connection*)>onReadTimeoutCallBack;
  typedef boost::function<void (connection*)>onWriteTimeoutCallBack;
  typedef boost::function<void (connection*)>onConnectionCloseCallBack;
  typedef boost::function<void (connection*)>onWriteCallBack;
  connection(int fd, struct event_base* base);
  ~connection();
  void init();
  void onMessage();
  int closeConnection();
  int doCloseConnection();
  void setKeepAlived(bool isKeepAlived);
  void setListener(listener* listener);
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

 private:
  void handleRead();
  void handleWrite();
  void handleError(connection*, short);
  static void eventEmptyCallBack(bufferevent* bufev, void* data);
  static void eventReadCallBack(bufferevent* bufev, void* data);
  static void eventWriteCallBack(bufferevent* bufev, void* data);
  static void eventErrorCallBack(bufferevent* bufev, short what, void* data);
  static void eventTimeoutCallBack(bufferevent* bufev, void* data);
  onMessageCallBack customizeOnMessageCallBack_;
  onConnectionCloseCallBack customizeOnConnectionCloseCallBack_;
  onWriteCallBack customizeOnWriteCallBack_;
  bool resetEvBuffer(struct evbuffer* buf);
  bool reuseBufferEvent(struct bufferevent* bufev);
  short bufferevent_get_enabled(struct bufferevent* bufev);
  int connfd_;
  CONN_STATE conn_state_;
  bufferevent* buf_;
  event_base* base_;
  bool keep_alived_;
  listener* listener_;
  void* privdata_;
};
#endif  // INTERNET_CONNECTION_H_
