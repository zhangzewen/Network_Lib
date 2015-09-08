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
  typedef boost::function<void(const std::string&, void*)> urlHandler;
  HttpServer(const std::string& host, int port);
  void start();
  void makeNewConnection(int, struct event_base*);
  bool registerUrl(const std::string&, const urlHandler& cb);
  void createHttpRequest(connection* con, char* buf, int len);
  void processHttpRequest(HttpRequest* request);
  void sendHttpResponse(HttpRequest* request, int stateCode, const char* content);
  bool isVailUrl(const std::string& url);
 private:
  std::map<std::string, urlHandler> urlHandleSet_;
  std::string host_;
  int port_;
  struct event_base* base_;
  listener* listener_;
};


#endif  // HTTP_HTTPSERVER_H_
