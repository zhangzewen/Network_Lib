#ifndef _ASYNCREDISCLIENT_H_INCLUDE__
#define _ASYNCREDISCLIENT_H_INCLUDE__

#include <string>
#include <hiredis.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <async.h>

class RedisAsyncClient
{
public:

	typedef boost::function<void(RedisAsyncClient*, redisReply*)> customizeCommandCallBack;
	typedef boost::function<void(RedisAsyncClient*, int)> customizeConnectCallBack;
	typedef boost::function<void(RedisAsyncClient*, int)> customizeDisConnectCallBack;
    //typedef void(*customizeCommandCallBack)(RedisAsyncClient*, redisReply*);
    //typedef void(*customizeConnectCallBack)(const RedisAsyncClient*, int);
    //typedef void(*customizeDisConnectCallBack)(const RedisAsyncClient*, int);
    RedisAsyncClient(struct event_base* base, const std::string& addr,
            int port, const std::string& dbname);
    std::string getDbName() const {
        return dbname_;
    }
    void connect();
    void disConnect();
    bool isConnected();
    int command(const customizeCommandCallBack& func, std::string cmd, ...);
    void setRedisAsyncClientConnectCallBack(const customizeConnectCallBack& cb);
    void setRedisAsyncClientDisConnectCallBack(const customizeDisConnectCallBack& cb);
    redisAsyncContext* getRedisAsyncContext() const {
        return context_;
    }
private:
    static void commandCallBack(struct redisAsyncContext* context, void* r, void* privdata);
    void commandCallBack(redisReply* reply, customizeCommandCallBack* cb);
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
