#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <adapters/libevent.h>
#include <event.h>
#include "redisClient.h"

RedisAsyncClient::RedisAsyncClient(struct event_base* base, const std::string& addr,
            int port, const std::string& dbname) : base_(base), addr_(addr),port_(port),
                dbname_(dbname)
{
}

void RedisAsyncClient::connect()
{
    context_ = redisAsyncConnect(addr_.c_str(), port_);
    context_->data = this;
    redisLibeventAttach(context_,base_);
    redisAsyncSetConnectCallback(context_,connectCallBack);
    redisAsyncSetDisconnectCallback(context_,disConnectCallBack);
}
void RedisAsyncClient::disConnect()
{
    if (isConnected())
    {
        redisAsyncDisconnect(context_);
    }
}
bool RedisAsyncClient::isConnected()
{
    return context_ && (context_->c.flags & REDIS_CONNECTED);
}

int RedisAsyncClient::command(const customizeCommandCallBack& func, std::string cmd, ...)
{
    if (!isConnected()) {
        return REDIS_ERR;
    }

	customizeCommandCallBack* p = new customizeCommandCallBack(func);
    va_list args;
    va_start(args, cmd);
    int ret = redisvAsyncCommand(context_, commandCallBack, p,
            cmd.c_str(), args);
    va_end(args);
    return ret;
}

void RedisAsyncClient::commandCallBack(struct redisAsyncContext* context, void* r,
        void* privdata)
{
    redisReply* reply = static_cast<redisReply*>(r);
    customizeCommandCallBack* cb = static_cast<customizeCommandCallBack*>(privdata);
    RedisAsyncClient* client = static_cast<RedisAsyncClient*>(context->data);
    client->commandCallBack(reply, cb);
}
void RedisAsyncClient::commandCallBack(redisReply* reply, customizeCommandCallBack* cb)
{
    (*cb)(this, reply);
	delete cb;
}
void RedisAsyncClient::connectCallBack(const struct redisAsyncContext* context,
        int status)
{
    RedisAsyncClient* client = static_cast<RedisAsyncClient*>(context->data);
    client->connectCallBack(status);
}
void RedisAsyncClient::disConnectCallBack(const struct redisAsyncContext* context,
        int status)
{
    RedisAsyncClient* client = static_cast<RedisAsyncClient*>(context->data);
    client->disConnectCallBack(status);
}

void RedisAsyncClient::connectCallBack(int status)
{
    if (status != REDIS_OK)
    {
        // do some logging
    }
    if (onConnect)
    {
        onConnect(this, status);
    }
}

void RedisAsyncClient::disConnectCallBack(int status)
{

    if (onDisConnect)
    {
        onDisConnect(this, status);
    }

}

void RedisAsyncClient::setRedisAsyncClientConnectCallBack(const customizeConnectCallBack& cb)
{
    onConnect = cb;
}
void RedisAsyncClient::setRedisAsyncClientDisConnectCallBack(const customizeDisConnectCallBack& cb)
{
    onDisConnect = cb;
}
