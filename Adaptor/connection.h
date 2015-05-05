#ifndef _ADAPTOR_CONNECTION_H_INCLUDED__
#define _ADAPTOR_CONNECTION_H_INCLUDED__

#include <map>
#include <string>
struct http_parser;
struct bufferevent;
struct http_parser_settings;
struct event_base;

class connection
{
public:
	typedef enum {
		CONNECT_INIT = 0,
		CONNECTING,
		CONNECTED,
		DISCONNECTING,
		DISCONNECTED,
	} CONN_STATE;
    typedef enum {
        READING = 0,
        READERROR,
        READDONE
    } READ_STATE;
    connection(int fd, struct event_base* base);
    ~connection();
    bool init();
    READ_STATE onMessage();
	int CloseConnection();
    int doCloseConnection();
    std::string key_;
    std::map<std::string, std::string> kvs_;
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
    READ_STATE read_state_;
    CONN_STATE conn_state_;
    bufferevent* buf_;
    http_parser* parser_;
    http_parser_settings* settings_;
    event_base* base_;
};
#endif //_ADAPTOR_CONNECTION_H_INCLUDED__
