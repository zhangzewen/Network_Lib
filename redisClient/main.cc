#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "redisClient.h"
#include <event.h>
#include <hiredis.h>

void selectCallBack(RedisAsyncClient* client, redisReply* reply)
{
    printf("Reply: [%s]", reply->str);
    client->disConnect();
}

void connectCallback(const RedisAsyncClient* client, int status) {
    redisAsyncContext* context = client->getRedisAsyncContext();
    if (status != REDIS_OK) {
        printf("Error: %s\n", context->errstr);
    }
    printf("Connected...\n");
}

void disconnectCallback(const RedisAsyncClient* client, int status) {
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
    client->setRedisAsyncClientConnectCallBack(connectCallback);
    client->setRedisAsyncClientDisConnectCallBack(disconnectCallback);
    client->connect();
    std::string db = client->getDbName();
    printf("Let's just select db %s....", db.c_str());
    client->command(selectCallBack, "select %s", db.c_str());
    event_base_dispatch(base);
    return 0;
}
