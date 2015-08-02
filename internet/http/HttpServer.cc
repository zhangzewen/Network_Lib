#include "HttpServer.h"
#include "listener.h"
#include "HttpRequest.h"
#include <event.h>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

HttpServer::HttpServer(const std::string& host, int port) : host_(host),
	port_(port), base_(event_base_new()),
   	listener_(new listener(host_, port_, base_)) {
}

void HttpServer::start()
{
	if (NULL == listener_) {
		std::cerr << "start error!" << std::endl;
	}
	listener_->setMakeNewConnectionCallBack(boost::bind(&HttpServer::makeNewConnection, this, _1, _2));
	listener_->start();
	event_base_loop(base_, 0);
}

bool HttpServer::makeNewConnection(int fd, struct event_base* base)
{
	connection* con = new connection(fd, base);
	con->setCustomizeOnMessageCallBack(boost::bind(&HttpServer::onMessage, this, _1, _2, _3));
	if (con->init()) {
		return true;
	}
	return false;
}


connection::CONN_STATE HttpServer::onMessage(connection* con, char*buf, int len)
{
	//std::cerr << buf << std::endl;
	HttpRequest request;
	request.init();
	request.parser(buf, len);
	return  connection::CON_READDONE;
}
