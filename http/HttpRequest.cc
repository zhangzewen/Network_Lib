#include <iostream>
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
  return 0;
}

static int on_url(http_parser*, const char* at, size_t len)
{
  std::string value(at, len);
  std::cout << value << std::endl;
  return 0;
}
static int on_status(http_parser*, const char* at, size_t len)
{
  std::string value(at, len);
  std::cout << value << std::endl;
  return 0;
}
static int on_header_field(http_parser*, const char* at, size_t len)
{
  std::string key(at, len);
  std::cout << key << std::endl;
  return 0;
}
static int on_header_value(http_parser*, const char* at, size_t len)
{
  std::string value(at, len);
  std::cout << value << std::endl;
  return 0;

}
static int on_headers_complete(http_parser* p)
{
  return 0;
}

static int on_message_complete(http_parser* p)
{
  return 0;
}

static int on_body(http_parser*, const char* at, size_t len)
{
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
