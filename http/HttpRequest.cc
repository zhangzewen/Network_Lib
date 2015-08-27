// Copyright [2015] <Zhang Zewen>

#include <assert.h>
#include <iostream>

#include "HttpRequest.h"
#include "connection.h"
#include "../util/util.h"


HttpRequest::HttpRequest() : state_(WAIT_REQUEST), conn_(NULL),
  parser_(NULL), server_(NULL)
{
}

HttpRequest::~HttpRequest()
{
  delete parser_;
}

static int on_message_begin(http_parser* p)
{
  assert(NULL != p);
  return 0;
}

static int on_url(http_parser* p, const char* at, size_t len)
{
  assert(NULL != p);
  assert(NULL != at);
  assert(len);
  return 0;
}

static int on_status(http_parser* p, const char* at, size_t len)
{
  assert(NULL != p);
  assert(NULL != at);
  assert(len);
  return 0;
}

static int on_header_field(http_parser* p, const char* at, size_t len)
{
  assert(NULL != p);
  assert(NULL != at);
  assert(len);
  HttpRequest* request = static_cast<HttpRequest*>(p->data);
  std::string tmp(at, len);
  tmp.push_back(':');
  request->addHeaderStream(tmp);
  return 0;
}

static int on_header_value(http_parser* p, const char* at, size_t len)
{
  assert(NULL != p);
  assert(NULL != at);
  assert(len);
  HttpRequest* request = static_cast<HttpRequest*>(p->data);
  std::string tmp(at, len);
  tmp.push_back(';');
  request->addHeaderStream(tmp);
  return 0;
}

static int on_headers_complete(http_parser* p)
{
  assert(NULL != p);
  HttpRequest* request = static_cast<HttpRequest*>(p->data);
  if (!request->parserHeaders()) {
    return 1;
  }
  request->setMethod(http_method_str(static_cast<enum http_method>(p->method)));
  if (request->getMethod() == "GET") {
    request->setState(HttpRequest::REQUEST_PARSER_DONE);
  }
  return 0;
}

static int on_message_complete(http_parser* p)
{
  assert(NULL != p);
  return 0;
}

static int on_body(http_parser* p, const char* at, size_t len)
{
  assert(NULL != p);
  assert(NULL != at);
  assert(len);
  std::string value(at, len);
  std::cout << value << std::endl;
  // body_length_ = len;
  return 0;
}

void HttpRequest::init()
{
  parser_ = new http_parser();
  if (NULL == parser_) {
    // close connection
  }
  http_parser_init(parser_, HTTP_REQUEST);
  parser_->data = this;
  parserSettings_.on_message_begin = on_message_begin;
  parserSettings_.on_url = on_url;
  parserSettings_.on_status = on_status;
  parserSettings_.on_header_field = on_header_field;
  parserSettings_.on_header_value = on_header_value;
  parserSettings_.on_headers_complete = on_headers_complete;
  parserSettings_.on_body = on_body;
  parserSettings_.on_message_complete = on_message_complete;
  conn_->setCustomizeOnMessageCallBack(
    boost::bind(&HttpRequest::parserRequest, this, _1, _2, _3));
}

int HttpRequest::parser(char* buf, int len)
{
  int nparsed = 0;
  nparsed = http_parser_execute(parser_, &parserSettings_, buf, len);
  if (parser_->upgrade) {
    // do something
  } else if (nparsed != len) {
    // Handle error. Usually just close the connection.
    state_ = REQUEST_PARSER_ERROR;
    return -1;
  }
  return 0;
}

void HttpRequest::addHeader(const std::string& key, const std::string& value)
{
  http_request_headers_.insert(std::make_pair(key, value));
}

bool HttpRequest::deleteHeader(const std::string& key)
{
  std::multimap<std::string, std::string>::iterator iter = http_request_headers_.find(key);
  if (iter != http_request_headers_.end()) {
    http_request_headers_.erase(iter);
    return true;
  }
  return false;
}

bool HttpRequest::parserHeaders()
{
  std::vector<std::string> tmp;
  split(headers_, ";", tmp);
  for (std::vector<std::string>::const_iterator iter = tmp.begin();
    iter != tmp.end();
    ++iter) {
    std::string::size_type pos = (*iter).find(':');
    if (pos == std::string::npos) {
      return false;
    }
    http_request_headers_.insert(std::make_pair((*iter).substr(0, pos),
      (*iter).substr(pos, (*iter).size() - pos)));
  }
  return true;
}

void HttpRequest::parserRequest(connection* conn, char* buf, int len)
{
  assert(conn);
  assert(buf);
  assert(len);
  if (state_ == WAIT_REQUEST && len > 0) {
    state_ = REQUEST_PARSERING;
  } else if (len == 0) {
    return;
  }
  int ret = parser(buf, len);
  if (ret == -1) {  // parser error!
    // close connection
  }

  if (state_ == REQUEST_PARSER_DONE) {  // parser done,then process
    httpResponseHeaders_.append("HTTP/1.1 200 OK\r\n");
    addResponseHeader("Content-lenght", "21");
    addResponseHeader("Server", "HttpServer v0.1");
    addResponseHeader("Content-Type", "text/html; charset=UTF-8");
    addResponseHeader("Date", "Wed, 26 Aug 2015 06:37:03 GMT");
    httpResponseHeaders_.append("<html>zhangjie</html>");
    conn_->doWrite(httpResponseHeaders_.c_str(), httpResponseHeaders_.size());
    conn_->tryWrite();
  }
  return;
}

void HttpRequest::closeRequest()
{
  conn_->closeConnection();
}

void HttpRequest::addResponseHeader(const std::string& key, const std::string& value)
{
  httpResponseHeaders_.append(key);
  httpResponseHeaders_.append(": ");
  httpResponseHeaders_.append(value);
  httpResponseHeaders_.append("\r\n");
}

void HttpRequest::addResponseHeaderDone()
{
  httpResponseHeaders_.append("\r\n");
}
