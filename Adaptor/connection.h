#ifndef _ADAPTOR_CONNECTION_H_INCLUDED__
#define _ADAPTOR_CONNECTION_H_INCLUDED__

struct http_parser;
struct bufferevent;
struct http_parser_settings;
struct event_base;

class connection
{
public:
    typedef enum {
        READBEGIN = 0,
        READING,
        READERROR,
        READDONE
    } READ_STATE;
    connection(int fd, struct event_base* base);
    ~connection();
    bool init();
    int onMessage();
private:
    int handleRead();
    int handleWrite();
    int handleError();
    int handleTimeOut();
    static void eventReadCallBack(bufferevent*, void*);
    static void eventWriteCallBack(bufferevent*, void*);
    static void eventErrorCallBack(bufferevent*, short, void*);
    static void eventTimeoutCallBack(bufferevent*, void*);
    int connfd_;
    int read_state_;
    bufferevent* buf_;
    http_parser* parser_;
    http_parser_settings* settings_;
    event_base* base_;
};
#endif //_ADAPTOR_CONNECTION_H_INCLUDED__
