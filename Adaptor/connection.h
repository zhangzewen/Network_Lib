#ifndef _ADAPTOR_CONNECTION_H_INCLUDED__
#define _ADAPTOR_CONNECTION_H_INCLUDED__

#include <map>
#include <string>
struct http_parser;
struct bufferevent;
struct http_parser_settings;
struct event_base;
class listener;

class connection
{
public:
	typedef enum {
		PARSEINIT,
		PARSEING,
		PARSEERROR,
		PARSEDONE
	}PARSER_STATE;
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
        READDONE,
        PROCESSING,
        PROCESSERROR,
        PROCESSDONE
    } READ_STATE;
    connection(int fd, struct event_base* base);
    ~connection();
    bool init();
    READ_STATE onMessage();
	int CloseConnection();
    int doCloseConnection();
    std::string key_;
    std::map<std::string, std::string> kvs_;
	PARSER_STATE parse_state_;
	int getFlowSource(std::string url);
    void setKeepAlived(bool isKeepAlived);
    void setListener(listener*);
    READ_STATE process(connection*);
private:
    void handleRead();
    void handleWrite();
    void handleError();
    void handleTimeOut();
    static void eventEmptyCallBack(bufferevent*, void*);
    static void eventReadCallBack(bufferevent*, void*);
    static void eventWriteCallBack(bufferevent*, void*);
    static void eventErrorCallBack(bufferevent*, short, void*);
    static void eventTimeoutCallBack(bufferevent*, void*);
    int connfd_;
	int current_flow_source_;//当前流量平台
    READ_STATE read_state_;
    CONN_STATE conn_state_;
    bufferevent* buf_;
    http_parser* parser_;
    http_parser_settings* settings_;
    event_base* base_;
    bool keep_alived_;
    listener* listener_;
};
#endif //_ADAPTOR_CONNECTION_H_INCLUDED__