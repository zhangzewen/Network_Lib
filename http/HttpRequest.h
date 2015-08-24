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
  void addHeader(const std::string& key, const std::string& value);
  bool deleteHeader(const std::string& key);
  void addHeaderStream(const std::string& elem) { headers_.append(elem);}
  bool parserHeaders();
  void* getPrivData() const { return privData_;}
  void setPrivData(void* data) { privData_ = data;}
  void http_parser_request(connection* conn, char* buf, int len);
  void tieConnection(connection* conn) {conn_ = conn;}
  void setMethod(const std::string& str) {method_ = str;}
  void setMethod(const char* str) {method_ = str;}
  std::string getMethod() const {return method_;}
  void setHttpVersion(const std::string& str) {http_version_ = str;}
  void setHttpVersion(const char* str) {http_version_ = str;}
  std::string getHttpVersion() const {return http_version_;}
  void setState(REQUEST_STATE state) {state_ = state;}
private:
	std::multimap<std::string, std::string> http_request_headers_;
  REQUEST_STATE state_;
  std::string headers_;
	std::string method_;
	std::string http_version_;
	std::string http_body_;
	int body_length_;
	connection* conn_;
	http_parser_settings parserSettings_;
	http_parser* parser_;
  void* privData_;
};
#endif
