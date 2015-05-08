#include <hiredis.h>
#include <async.h>
#include <adapters/libevent.h>
#include <event2/event.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include "redisClient.h"

RedisAsyncClient::RedisAsyncClient(struct event_base* base, const std::string& addr,
            int port, const std::string& dbname) : base_(base), addr_(addr),port_(port),
                dbname_(dbname)
{
}

void RedisAsyncClient::connect()
{
    context_ = redisAsyncConnect(addr_.c_str(), port_);
    context_->ev.data = this;
    redisLibeventAttach(context_,base_);
    redisAsyncSetConnectCallback(context_,connectCallBack);
    redisAsyncSetDisconnectCallback(context_,disConnectCallBack);
}
bool RedisAsyncClient::isConnected()
{
    return false;
}

int RedisAsyncClient::command(customizeCallBack func, std::string cmd, ...)
{
    if (isConnected()) {
        return REDIS_ERR;
    }
    va_list args;
    va_start(args, cmd);
    int ret = redisvAsyncCommand(context_, commandCallBack, (void*)func,
            cmd.c_str(), args);
    va_end(args);
    return ret;
}

void RedisAsyncClient::commandCallBack(struct redisAsyncContext* context, void* r,
        void* privdata)
{
    redisReply* reply = static_cast<redisReply*>(r);
    customizeCallBack cb = (customizeCallBack)privdata;
    RedisAsyncClient* client = static_cast<RedisAsyncClient*>(context->data);
    client->commandCallBack(reply, cb);
}
void RedisAsyncClient::commandCallBack(redisReply* reply, customizeCallBack cb)
{
    cb(this, reply);
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
}

void RedisAsyncClient::disConnectCallBack(int status)
{
}

