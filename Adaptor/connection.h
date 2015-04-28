#ifndef _ADAPTOR_CONNECTION_H_INCLUDED__
#define _ADAPTOR_CONNECTION_H_INCLUDED__

struct http_parser;
struct bufferevent;

class connection
{
public:
    connection();
    ~connection();
private:
    bufferevent* buf;
    http_parser* parser;
};
#endif //_ADAPTOR_CONNECTION_H_INCLUDED__
