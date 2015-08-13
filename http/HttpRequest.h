#ifndef NETWORK_HTTPREQUEST_H_INCLUDED
#define NETWORK_HTTPREQUEST_H_INCLUDED

#include <map>
#include <string>
#include "http_parser.h"

class connection;

class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();
	bool init();
	int parser(char* buf, int len);
  void addHeader(const std::string& key, const std::string& value);
  bool deleteHeader(const std::string& key);
  void addHeaderStream(const std::string& elem) {
    headers_.append(elem);
  }
  bool parserHeaders();
private:
	std::multimap<std::string, std::string> http_request_headers_;
  std::string headers_;
	std::string method_;
	std::string http_version_;
	std::string http_body_;
	int body_length_;
	connection* conn_;
	http_parser_settings parserSettings_;
	http_parser* parser_;
};
#endif
