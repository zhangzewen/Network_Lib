#ifndef NETWORK_HTTP_H_INCLUDED_
#define NETWORK_HTTP_H_INCLUDED_

#include <string>
#include "connection.h"
class listener;
class event_base;

class HttpServer
{
public:
	HttpServer(const std::string&, int port);
	void start();
	bool makeNewConnection(int fd, struct event_base* base);
	connection::CONN_STATE onMessage(connection*, char*, int);
private:
	std::string host_;
	int port_;
	struct event_base* base_;
	listener* listener_;
};


#endif //NETWORK_HTTP_H_INCLUDED_
