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
		CON_CONNECTING,
		CON_CONNECTED,
		CON_DISCONNECTING,
		CON_DISCONNECTED,
		CON_IDLE,
		CON_READING,
		CON_WRITTING
	} CONN_STATE;
    typedef enum {
        READING = 0,
        READERROR,
        READDONE,
        PROCESSING,
        PROCESSERROR,
        PROCESSDONE
    } READ_STATE;
	typedef READ_STATE (*onMessageCallBack)(connnection*, char*, int);
    connection(int fd, struct event_base* base);
    ~connection();
    bool init();
    READ_STATE onMessage();
	int CloseConnection();
    int doCloseConnection();
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
	onMessageCallBack customizeOnMessageCallBack_;
    int connfd_;
    READ_STATE read_state_;
    CONN_STATE conn_state_;
    bufferevent* buf_;
    event_base* base_;
    bool keep_alived_;
    listener* listener_;
};
#endif //_ADAPTOR_CONNECTION_H_INCLUDED__
