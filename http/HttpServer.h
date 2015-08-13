#ifndef NETWORK_HTTP_H_INCLUDED_
#define NETWORK_HTTP_H_INCLUDED_

#include <string>
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
	HttpServer(const std::string&, int port);
	void start();
	void makeNewConnection(int, struct event_base*);
	void onMessage(connection*, char*, int);
  void onParserRequest(connection* , char* , int);
private:
	std::string host_;
	int port_;
	struct event_base* base_;
	listener* listener_;
};


#endif //NETWORK_HTTP_H_INCLUDED_
