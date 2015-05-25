#ifndef SRC_HTTPSERVER_H_INCLUDED
#define SRC_HTTPSERVER_H_INCLUDED
#include <string>

struct event_base;
struct evhttp;
struct evhttp_bound_socket;

class HttpServer
{
public:
    typedef void (*pathHandle)(struct evhttp_request *req, void *arg);
    HttpServer(struct event_base* base, const std::string& host, int port);
    ~HttpServer();
    void registePath(const std::string& path,
            pathHandle handle);
    void listen();
private:
    struct event_base* base_;
    struct evhttp* httpServer_;
    struct evhttp_bound_socket* handle_;
    std::string host_;
    int port_;
};
#endif
