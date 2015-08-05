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
    CON_CONNECTING,
    CON_CONNECTED,
    CON_DISCONNECTING,
    CON_DISCONNECTED,
    CON_IDLE,//  After connected, this state is between registed Read Event  and a read event fired
    CON_READING,
    CON_READERROR,
    CON_READDONE,
    CON_PROCESSING,
    CON_PROCESSERROR,
    CON_PROCESSDONE,
    CON_WRITTING,
    CON_WRITEERROR,
    CON_WRITEDONE
  } CONN_STATE;
  //typedef CONN_STATE (*onMessageCallBack)(connection*, char*, int);
  typedef boost::function<CONN_STATE(connection*, char*, int)> onMessageCallBack;
  typedef boost::function<CONN_STATE(connection*)> onProcessCallBack;
  //typedef CONN_STATE (*OnProcessCallBack)(connection*);
  connection(int fd, struct event_base* base);
  ~connection();
  bool init();
  CONN_STATE onMessage();
  CONN_STATE doProcess();
  int closeConnection(); 
  int doCloseConnection();
  void setKeepAlived(bool isKeepAlived);
  void setListener(listener*);
  bool isKeepAlived();
  //CON_STATE process(connection*); why should there have a  process method ,this should be customized
  bool reuseConnection();
  void enableRead();
  void enableWrite();
  void disableRead();
  void disableWrite();
  void disableReadWrite();
  void setPrivData(void *);
  void* getPrivData() const;
  void setCustomizeOnMessageCallBack(const onMessageCallBack& cb) {
    customizeOnMessageCallBack_ = cb;
  }
  void setCustomizeOnProcessCallBack(const onProcessCallBack& cb) {
    customizeOnProcessCallBack_ = cb;
  }
  void startRead();
  void startWrite();
  void readDone();
  void writeDone();
  int tryWrite();
private:
  void handleRead();
  void handleWrite();
  void handleError();
  void handleTimeOut();
  static void eventEmptyCallBack(bufferevent*, void*);
  static void eventReadCallBack(bufferevent*, void*);
  static void eventWriteCallBack(bufferevent*, void*);
  static void eventErrorCallBack(bufferevent*, short, void*);
  static void eventTimeoutCallBack(bufferevent*, void*);
  onMessageCallBack customizeOnMessageCallBack_;
  onProcessCallBack customizeOnProcessCallBack_;
  bool reuseEvBuffer(struct evbuffer*);
  bool reuseBufferEvent(struct bufferevent*);
  short bufferevent_get_enabled(struct bufferevent*);
  int connfd_;
  CONN_STATE conn_state_;
  bufferevent* buf_;
  event_base* base_;
  bool keep_alived_;
  listener* listener_;
  void* privdata_;
};
#endif //INTERNET_CONNECTION_H_