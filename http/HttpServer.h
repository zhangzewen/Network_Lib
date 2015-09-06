// Copyright [2015] <Zhang Zewen>

#ifndef HTTP_HTTPSERVER_H_
#define HTTP_HTTPSERVER_H_

#include <string>
#include <map>
#include <boost/function.hpp>
//#include "connection.h"

class listener;
class event_base;
class connection;
class HttpRequest;

class HttpServer
{
 public:
  HttpServer(const std::string& host, int port);
  void start();
  void makeNewConnection(int, struct event_base*);
  void onMessage(connection* con, char* buf, int len);
  void onParserRequest(connection* con, char* buf, int len);
  typedef boost::function<void()> urlHandler;
  bool registerUrl(const std::string&, const urlHandler& cb);
  void processRequest(HttpRequest* request);
 private:
  std::map<std::string, urlHandler> urlHandleSet_;
  std::string host_;
  int port_;
  struct event_base* base_;
  listener* listener_;
};


#endif  // HTTP_HTTPSERVER_H_
