#include "HttpServer.h"
#include <evhttp.h>
#include <string>

HttpServer::HttpServer(struct event_base* base,
        const std::string& host, int port) : base_(base),
    httpServer_(evhttp_new(base_)), host_(host), port_(port)
{

}

void HttpServer::registePath(const std::string& path, pathHandle handle)
{
    evhttp_set_cb(httpServer_, path.c_str(), handle, NULL);
}
