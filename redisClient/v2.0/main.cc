#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "redisClient.h"
#include <event.h>
#include <hiredis.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>


void setKeyCallBack(RedisAsyncClient* client, redisReply* reply, std::string* message)
{
    printf("Reply: [%s] : %s\n", reply->str, message->c_str());
}

void selectCallBack(RedisAsyncClient* client, redisReply* reply)
{
    printf("Reply: [%s]", reply->str);
	std::string* one = new std::string("zhangjieNewOne");
	std::string* two = new std::string("zhangjieNewTwo");
	client->command(boost::bind(setKeyCallBack, _1, _2, one), "set oneNew %s", "NEWONE");
	client->command(boost::bind(setKeyCallBack, _1, _2, two), "set twoNew %s", "NEWTWO");
}

void connectCallback(RedisAsyncClient* client, int status) {
    redisAsyncContext* context = client->getRedisAsyncContext();
    if (status != REDIS_OK) {
        printf("Error: %s\n", context->errstr);
        return;
    }
    printf("Connected...\n");
    std::string db = client->getDbName();
    printf("Let's just select db %s....\n", db.c_str());
    client->command(selectCallBack, "select %s", db.c_str());
}

void disconnectCallback(RedisAsyncClient* client, int status) {
    redisAsyncContext* context = client->getRedisAsyncContext();
    if (status != REDIS_OK) {
        printf("Error: %s\n", context->errstr);
        return;
    }
    printf("Disconnected...\n");
}



int main(int argc, char** argv)
{
    struct event_base* base = event_base_new();
    if (NULL == base) {
        exit(1);
    }
    RedisAsyncClient* client = new RedisAsyncClient(base, std::string("127.0.0.1"), 6379, std::string("0"));
    client->setRedisAsyncClientConnectCallBack(boost::bind(connectCallback, _1, _2));
    client->setRedisAsyncClientDisConnectCallBack(boost::bind(disconnectCallback, _1, _2));
    client->connect();
    event_base_dispatch(base);
    return 0;
}
