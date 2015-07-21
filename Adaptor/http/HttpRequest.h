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
private:
	std::map<std::string, std::string> http_request_headers_;
	std::string method_;
	std::string http_version_;
	std::string http_body_;
	int body_length_;
	connection* conn_;
	http_parser_settings parserSettings_;
	http_parser* parser_;
};
#endif
