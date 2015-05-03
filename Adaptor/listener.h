#ifndef __ADAPTOR_LISTENER_H_INCLUDED__
#define __ADAPTOR_LISTENER_H_INCLUDED__


#include <string>

struct event_base;

class listener
{
public:
	listener(struct event_base* base);
	~listener();
    void start();
    struct event_base* getEventBase() {
        return base_;
    }
    void makeConnection(int, struct event_base*);
private:
    static void listenCallBack(int fd, short event, void* arg);
	int createSocketAndListen();
    struct event_base* base_;
    int listenfd_;
    struct event* ev_;
    //std::string host_;
    //int port_;
};

#endif //__ADAPTOR_LISTENER_H_INCLUDED__
