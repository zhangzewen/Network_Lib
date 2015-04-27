#ifndef __ADAPTOR_LISTENER_H_INCLUDED__
#define __ADAPTOR_LISTENER_H_INCLUDED__

class listener
{
public:
    listener(struct event_base* base);
    listener(struct event_base* base, const std::string& host, int port);
private:
    struct event_base* base;
    int listenfd_;
    struct event* ev_;
    std::string host_;
    int port_;
};

#endif //__ADAPTOR_LISTENER_H_INCLUDED__
