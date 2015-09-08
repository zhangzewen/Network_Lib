// Copyright [2015] <Zhang Zewen>

#include <assert.h>
#include <iostream>
#include <event.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <glog/logging.h>
#include "HttpRequest.h"
#include "HttpServer.h"
#include "listener.h"

HttpServer::HttpServer(const std::string& host, int port) : host_(host),
  port_(port), base_(event_base_new()),
  listener_(new listener(host_, port_, base_)) {
  }


/**
  @param
*/
void HttpServer::start()
{
  if (NULL == listener_) {
    LOG(ERROR) << "start error!";
  }
  listener_->setMakeNewConnectionCallBack(
      boost::bind(&HttpServer::makeNewConnection, this, _1, _2));
  listener_->start();
  event_base_loop(base_, 0);
}

/**
  @param
*/
void HttpServer::makeNewConnection(int fd, struct event_base* base)
{
  connection* con = new connection(fd, base);
  if (NULL == con) {
    // just close fd
    LOG(ERROR) << "malloc connection error!";
    close(fd);
    return;
  }
  con->setCustomizeOnMessageCallBack(
      boost::bind(&HttpServer::createHttpRequest, this, _1, _2, _3));
  con->init();
}


/**
  @param
*/
void HttpServer::createHttpRequest(connection* con, char*buf, int len)
{
  if (NULL == buf || 0 == len) {
    return;
  }

  HttpRequest *request = new HttpRequest();
  if (NULL == request) {
    //  do some close work
    con->closeConnection(); 
  }
  request->tieServer(this);
  con->setPrivData(request);
  request->tieConnection(con);
  request->init();
  if (len) {
    request->parserRequest(con, buf, len);
  }
}

//  /**
//    this function is the first step of process a http request: parse request, 
//    using a third_party lib to parser the http request, this is a Finite-state
//    machine ,when we done parser, we will step next to process request
//    @param 
//  */
//  void HttpServer::parserRequest(connection* conn, char* buf, int len)
//  {
//    assert(NULL != conn);
//    assert(NULL != buf);
//    assert(0 != len);
//    HttpRequest* request = static_cast<HttpRequest*>(conn->getPrivData());
//    request->parser(buf, len);
//  }


/**
  when the parser job has already done and no error happend
  the server according to the request to answer it

  @request request that will be processed and answered
  return null
*/
void HttpServer::processHttpRequest(HttpRequest* request)
{
  // product http response and register writeCallBack function
  assert(request);
  sendHttpResponse(request, 200, "OK");
}

#if 0
connection::CONN_STATE HTTPsERVER::processRequest(connection* con)
{
  HttpReqest* request  = static_cast<HttpReqeust*>conn->privdata;
}
#endif


bool HttpServer::isVailUrl(const std::string& url)
{
  typedef std::map<std::string, urlHandler>::const_iterator UrlIter;
  UrlIter iter = urlHandleSet_.find(url);
  if (iter == urlHandleSet_.end()) {
    return false;
  }
  return true;
}

void HttpServer::sendHttpResponse(HttpRequest* request, int stateCode, const char* content)
{
  assert(request);
  assert(stateCode);
  assert(content);
}


