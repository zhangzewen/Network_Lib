// Copyright [2015] <Zhang Zewen>

#include <assert.h>
#include <iostream>
#include <event.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "HttpRequest.h"
#include "HttpServer.h"
#include "listener.h"

HttpServer::HttpServer(const std::string& host, int port) : host_(host),
  port_(port), base_(event_base_new()),
  listener_(new listener(host_, port_, base_)) {
  }

void HttpServer::start()
{
  if (NULL == listener_) {
    std::cerr << "start error!" << std::endl;
  }
  listener_->setMakeNewConnectionCallBack(
      boost::bind(&HttpServer::makeNewConnection, this, _1, _2));
  listener_->start();
  event_base_loop(base_, 0);
}

void HttpServer::makeNewConnection(int fd, struct event_base* base)
{
  connection* con = new connection(fd, base);
  con->setCustomizeOnMessageCallBack(
      boost::bind(&HttpServer::onMessage, this, _1, _2, _3));
  con->init();
}


void HttpServer::onMessage(connection* con, char*buf, int len)
{
  if (NULL == buf || 0 == len) {
    return;
  }
  HttpRequest *request = new HttpRequest();
  if (NULL == request) {
    // close request
  }
  request->tieServer(this);
  con->setPrivData(request);
  request->tieConnection(con);
  request->init();
  if (len) {
    request->parserRequest(con, buf, len);
  }
}

void HttpServer::onParserRequest(connection* conn, char* buf, int len)
{
  assert(NULL != conn);
  assert(NULL != buf);
  assert(0 != len);
  HttpRequest* request = static_cast<HttpRequest*>(conn->getPrivData());
  request->parser(buf, len);
}

#if 0
connection::CONN_STATE HTTPsERVER::processRequest(connection* con)
{
  HttpReqest* request  = static_cast<HttpReqeust*>conn->privdata;
}
#endif
