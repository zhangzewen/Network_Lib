// Copyright [2015] <Zhang Zewen>

#ifndef HTTP_HTTPREQUEST_H_
#define HTTP_HTTPREQUEST_H_

#include <map>
#include <string>

#include "http_parser.h"

// for now, just HTTP GET, HEAD, POST
// when do crawl, HTTP HEAD can identiry whether the page we
// crawled is modified or not

class connection;
class HttpServer;

class HttpRequest
{
 public:
  HttpRequest();
  ~HttpRequest();
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
  void init();
  int parser(char* buf, int len);
  bool parserHeaders();
  void parserRequest(connection* conn, char* buf, int len);
  void addHeader(const std::string& key, const std::string& value);
  bool deleteHeader(const std::string& key);
  void addHeaderStream(const std::string& elem) {headers_.append(elem);}
  void tieConnection(connection* conn) {conn_ = conn;}
  connection* getConnection() const { return conn_;}
  void tieServer(HttpServer* server) {server_ = server;}
  HttpServer* getHttpServer() const {return server_;}
  void setMethod(const std::string& str) {method_ = str;}
  void setMethod(const char* str) {method_ = str;}
  std::string getMethod() const {return method_;}
  void setHttpVersion(const std::string& str) {http_version_ = str;}
  void setHttpVersion(const char* str) {http_version_ = str;}
  std::string getHttpVersion() const {return http_version_;}
  void setState(REQUEST_STATE state) {state_ = state;}
  REQUEST_STATE getState() const { return state_;}
  void setUrl(const std::string& url) {url_ = url;}
  void setUrl(const char* url) { url_ = url;}
  std::string getUrl() const {return url_;}
  void closeRequest();
  void addResponseHeader(const std::string& key, const std::string& value);
  void addResponseHeaderDone();

 private:
  std::multimap<std::string, std::string> http_request_headers_;
  REQUEST_STATE state_;
  std::string headers_;
  std::string method_;
  std::string http_version_;
  std::string http_body_;
  std::string url_;
  std::string httpResponseHeaders_;
  int body_length_;
  connection* conn_;
  http_parser_settings parserSettings_;
  http_parser* parser_;
  HttpServer* server_;
  // std::vector<char> request_headers_; //request stream
  // std::vector<char> response_headers_; //response stream
};
#endif  // HTTP_HTTPREQUEST_H_
