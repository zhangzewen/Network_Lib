#ifndef _ASYNCREDISCLIENT_H_INCLUDE__
#define _ASYNCREDISCLIENT_H_INCLUDE__

struct event_base;
struct redisReply;
struct redisAsyncContext;

class RedisAsyncClient
{
public:
    typedef void(*customizeCallBack)(RedisAsyncClient*, redisReply*);
    typedef void(*customizeConnectCallBack)(RedisAsyncClient*, int);
    typedef void(*customizeDisConnectCallBack)(RedisAsyncClient*, int);
    RedisAsyncClient(struct event_base* base, const std::string& addr,
            int port, const std::string& dbname);
    void connect();
    void disConnect();
    bool isConnected();
    int command(customizeCallBack func, std::string cmd, ...);
    void setRedisAsyncClientConnectCallBack(customizeConnectCallBack cb);
    void setRedisAsyncClientDisConnectCallBack(customizeDisConnectCallBack cb);
private:
    static void commandCallBack(struct redisAsyncContext* context, void* r, void* privdata);
    void commandCallBack(redisReply* reply, customizeCallBack cb);
    static void connectCallBack(const struct redisAsyncContext*, int status);
    void connectCallBack(int);
    static void disConnectCallBack(const struct redisAsyncContext*, int status);
    void disConnectCallBack(int);

    customizeConnectCallBack onConnect;
    customizeDisConnectCallBack onDisConnect;
    struct event_base* base_;
    std::string addr_;
    int port_;
    std::string dbname_;
    redisAsyncContext* context_;;
};


#endif //_ASYNCREDISCLIENT_H_INCLUDE__
