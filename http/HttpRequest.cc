#include <iostream>
#include <assert.h>
#include "HttpRequest.h"


HttpRequest::HttpRequest()
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
  request->parserHeaders();
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
  //body_length_ = len;
  return 0;
}

bool HttpRequest::init()
{
  parser_ = new http_parser();
  if (NULL == parser_) {
    return false;
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
  return true;
}

int HttpRequest::parser(char* buf, int len)
{
  int nparsed = 0;
  nparsed = http_parser_execute(parser_, &parserSettings_, buf, len);
  if (parser_->upgrade) {
    //do something
  } else if (nparsed != len) {
    //Handle error. Usually just close the connection.
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
  if ( iter != http_request_headers_.end()) {
    http_request_headers_.erase(iter);
    return true;
  }
  return false;
}

bool HttpRequest::parserHeaders()
{
  return true;
}
