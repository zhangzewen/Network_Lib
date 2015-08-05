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
	if (con->init()) {
		return true;
	}
	con->setCustomizeOnMessageCallBack(boost::bind(&HttpServer::onMessage, this, _1, _2, _3));
  con->setCustomizeOnProcessCallBack(boost::bind(&HttpServer::processRequest, this, _1));
	return false;
}


connection::CONN_STATE HttpServer::onMessage(connection* con, char*buf, int len)
{
	HttpRequest request;
	request.init();
	request.parser(buf, len);
	return  connection::CON_READDONE;
}

connection::CONN_STATE HTTPsERVER::processRequest(connection* con)
{
  HttpReqest* request  = static_cast<HttpReqeust*>conn->privdata;
}
