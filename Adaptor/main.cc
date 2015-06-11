#include <event.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
//#include "http_parser.h"
#include "listener.h"



int main(int argc, char** argv)
{
#if 0
    int listenfd = 0;
    if (listenfd < 0) {
        std::cerr << "Create and listen fd error!" << std::endl;
    }
#endif
#if 0
    struct event_base* base;
    base  = event_base_new();
    if (NULL == base) {
        std::cerr << "create event base error" << std::endl;
        return 0;
    }
    listener* listen = new listener(base);
    listen->start();
    event_base_dispatch(base);
#endif
    return 0;
}
