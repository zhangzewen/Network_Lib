#ifndef INTERNET_ECHO_H
#define INTERNET_ECHO_H

#include "connection.h"
#include "listener.h"
#include <event.h>
#include <iostream>

class EchoServer
{
public:
	EchoServer(const std::string&, int port);
	void start();
	bool makeNewConnection(int fd, struct event_base* base);
	connection::CONN_STATE onMessage(connection*, char*, int);
	connection::CONN_STATE processRequest(connection*);
private:
	std::string host_;
	int port_;
	struct event_base* base_;
	listener* listener_;
};

EchoServer::EchoServer(const std::string& host, int port) : host_(host),
	port_(port), base_(event_base_new()),
   	listener_(new listener(host_, port_, base_)) {
}

void EchoServer::start()
{
	if (NULL == listener_) {
		std::cerr << "start error!" << std::endl;
	}
	listener_->setMakeNewConnectionCallBack(boost::bind(&EchoServer::makeNewConnection, this, _1, _2));
	listener_->start();
	event_base_loop(base_, 0);
}

bool EchoServer::makeNewConnection(int fd, struct event_base* base)
{
	connection* con = new connection(fd, base);
	con->setCustomizeOnMessageCallBack(boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
	if (con->init()) {
		return true;
	}
	return false;
}


connection::CONN_STATE EchoServer::onMessage(connection* conn, char*buf, int len)
{
  assert(conn != NULL);
  assert(buf != NULL);
  assert(len != 0);
  std::cout << buf << std::endl;
	return  connection::CON_READDONE;
}

#endif //INTERNET_ECHO_H
