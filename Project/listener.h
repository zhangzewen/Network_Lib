#ifndef __ADAPTOR_LISTENER_H_INCLUDED__
#define __ADAPTOR_LISTENER_H_INCLUDED__


#include <string>
#include "connection.h"

struct event_base;

class listener
{
public:
	//typedef connection::CONN_STATE (*onMessageCallBack)(connection*, char*, int);
	//typedef bool(*makeNewConnection)(int, struct event_base*);
	typedef boost::function<bool(int, struct event_base*)> makeNewConnection;
	listener(const std::string&, int port, struct event_base* base);
	listener(const std::string&, int port);
	~listener();
    void start();
    struct event_base* getEventBase() const {
        return base_;
    }
	void setEventBase(struct event_base* base) {
		base_ = base;
	}
    void setMakeNewConnectionCallBack(const makeNewConnection& cb) {
		makeNewConnectionCallBack_ = cb;
	}
	//makeNewConnection getMakeNewConnectionCallBack() const {
	//	return makeNewConnectionCallBack_;
	//}
	//void setConnectionOnMessageCallBack(onMessageCallBack cb) {
	//	onMessage_ = cb;
	//}
	//onMessageCallBack getOnMessageCallBack() const {
	//	return onMessage_;
	//}
	bool doMakeConnection(int connfd);
private:
    static void listenCallBack(int fd, short event, void* arg);
	int createSocketAndListen();
    int listenfd_;
    std::string host_;
    int port_;
    struct event_base* base_;
    struct event* ev_;
	makeNewConnection makeNewConnectionCallBack_;
	//onMessageCallBack onMessage_;
};

#endif //__ADAPTOR_LISTENER_H_INCLUDED__
