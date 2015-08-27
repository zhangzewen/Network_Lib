// Copyright [2015] <Zhang Zewen>

#ifndef HTTP_HTTPSERVER_H_
#define HTTP_HTTPSERVER_H_

#include <string>
#include <map>
#include "connection.h"
class listener;
class event_base;

class HttpServer
{
 public:
  typedef enum {
    WAIT_REQUEST,
    REQUEST_PARSERING,
    REQUEST_PARSER_ERROR,
    REQUEST_PARSER_DONE,
    REQUEST_PROCESSING,
    REQUEST_PROCESS_ERROR,
    REQUEST_PROCESS_DONE,
    REQUEST_SENT_RESPONSE,
    REQUEST_SENT_RESPONSE_ERROR,
    REQUEST_SENT_RESPONSE_DONE,
  }REQUEST_STATE;
  HttpServer(const std::string& host, int port);
  void start();
  void makeNewConnection(int, struct event_base*);
  void onMessage(connection* con, char* buf, int len);
  void onParserRequest(connection* con, char* buf, int len);
  typedef boost::function<void()> urlHandler;
  bool registerUrl(const std::string&, const urlHandler& cb);

 private:
  std::map<std::string, urlHandler> urlHandleSet_;
  std::string host_;
  int port_;
  struct event_base* base_;
  listener* listener_;
};


#endif  // HTTP_HTTPSERVER_H_
