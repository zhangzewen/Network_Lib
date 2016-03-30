#include "reactor.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <glog/logging.h>

#define return_ok "YES, it's work\r\n\r\n"

using namespace std::placeholders;

class Echo
{
public:
    void Print(std::shared_ptr<Event> ev) {
        assert(NULL != ev);
        std::cout<<"Print()" << std::endl;
    }

};

struct cmp
{
    bool operator()(Timer& a1, Timer& b1)
    {
        return a1 <= b1 ? false : true;
    }
};

void visit(Timer timer, Event ev)
{
    std::cout << "timer: " << timer.convertToMilliseconds() << std::endl;
}

int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    std::shared_ptr<Dispatcher> dis(new Dispatcher());
    Echo echoServer;

    std::shared_ptr<Epoll> poller (new Epoll());
    poller->init();
    dis->setPoller(poller);
    //dis->runAt("fuck me!", 10, std::bind(&Echo::Print, &echoServer, _1));
    dis->runEvery("fuck me!", 10, std::bind(&Echo::Print, &echoServer, _1));
    dis->loop();

//    RBTree<Timer, Event, cmp> timeout;
//    Timer t1(1);
//    Event e1("e1");
//
//    Timer t2(1);
//    Event e2("e2");
//
//    Timer t3(2);
//    Event e3("e3");
//
//    Timer t4(3);
//    Event e4("e4");
//
//    Timer t5(4);
//    Event e5("e5");
//
//    Timer t6(5);
//    Event e6("e6");
//
//    timeout.insert(t6, e6);
//    timeout.insert(t1, e1);
//    timeout.insert(t2, e2);
//    timeout.insert(t4, e4);
//    timeout.insert(t3, e3);
//    timeout.insert(t5, e5);
//    timeout.Print(visit);
//
//    RBTree<Timer, Event, cmp>::Node* node = timeout.search(t4);
//    Event em1 = node->value;
//    Timer tm1 = node->key;
//    std::cout << "==============" << std::endl;
//    timeout.Delete(t4);
//    timeout.Print(visit);
//



//    Timer time;
//    Timer timeStep(10);
//    Timer timeout = time + timeStep;
//    std::cout << (timeout < time) << std::endl;
	return 0;
}


