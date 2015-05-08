#ifndef _ASYNCREDISCLIENT_H_INCLUDE__
#define _ASYNCREDISCLIENT_H_INCLUDE__

struct event_base;
struct redisReply;
struct redisAsyncContext;

class RedisAsyncClient
{
public:
    typedef void(*customizeCallBack)(RedisAsyncClient*, redisReply*);
    RedisAsyncClient(struct event_base* base, const std::string& addr,
            int port, const std::string& dbname);
    void connect();
    bool isConnected();
    int command(customizeCallBack func, std::string cmd, ...);
private:
    static void commandCallBack(struct redisAsyncContext* context, void* r, void* privdata);
    static void connectCallBack(const struct redisAsyncContext*, int status);
    void connectCallBack(int);
    static void disConnectCallBack(const struct redisAsyncContext*, int status);
    void disConnectCallBack(int);
    void commandCallBack(redisReply* reply, customizeCallBack cb);
    struct event_base* base_;
    std::string addr_;
    int port_;
    std::string dbname_;
    redisAsyncContext* context_;;
};


#endif //_ASYNCREDISCLIENT_H_INCLUDE__
