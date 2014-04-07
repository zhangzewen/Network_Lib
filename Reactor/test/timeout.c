#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "event.h"
#include "event_base.h"

#define evtimer_set(ev, cb, arg, name)  event_set(ev, -1, 0, cb, arg, name)

static void timeout_cb1(int fd, short event, void *arg)
{
	assert(fd != 0);
	assert(event != 0);
	assert(arg != NULL);
	printf("1 timeout now!\n");
}

static void timeout_cb2(int fd, short event, void *arg)
{
	assert(fd != 0);
	assert(event != 0);
	assert(arg != NULL);
	printf("2 timeout now!\n");
}

static void timeout_cb3(int fd, short event, void *arg)
{
	assert(fd != 0);
	assert(event != 0);
	assert(arg != NULL);
	printf("3 timeout now!\n");
}

static void timeout_cb4(int fd, short event, void *arg)
{
	assert(fd != 0);
	assert(event != 0);
	assert(arg != NULL);
	printf("4 timeout now!\n");
} 

static void timeout_cb5(int fd, short event, void *arg)
{
	assert(fd != 0);
	assert(event != 0);
	assert(arg != NULL);
	printf("5 timeout now!\n");
}

static void timeout_cb6(int fd, short event, void *arg)
{
	assert(fd != 0);
	assert(event != 0);
	assert(arg != NULL);
	printf("6 timeout now!\n");
}

static void timeout_cb7(int fd, short event, void *arg)
{
	assert(fd != 0);
	assert(event != 0);
	assert(arg != NULL);
	printf("7 timeout now!\n");
}




int main(int argc, char *argv[])
{

	assert(argc != 0);
	assert(argv != NULL);
	fprintf(stderr, "=====================begin=================\n");
	struct event timeout1;
	struct event timeout2;
#if 1
	struct event timeout3;
	struct event timeout4;
	struct event timeout5;
	struct event timeout6;
	struct event timeout7;
#endif

	struct timeval tv1;
	struct timeval tv2;
#if 1
	struct timeval tv3;
	struct timeval tv4;
	struct timeval tv5;
	struct timeval tv6;
	struct timeval tv7;
#endif
	event_init();
	
	evtimer_set(&timeout1, timeout_cb1, &timeout1, "timeout_1");
	evtimer_set(&timeout2, timeout_cb2, &timeout2, "timeout_2");
#if 1
	evtimer_set(&timeout3, timeout_cb3, &timeout3, "timeout_3");
	evtimer_set(&timeout4, timeout_cb4, &timeout4, "timeout_4");
	evtimer_set(&timeout5, timeout_cb5, &timeout5, "timeout_5");
	evtimer_set(&timeout6, timeout_cb6, &timeout6, "timeout_6");
	evtimer_set(&timeout7, timeout_cb7, &timeout7, "timeout_7");
#endif

	tv1.tv_sec = 2;
	tv1.tv_usec = 0;

	tv2.tv_sec = 4;
	tv2.tv_usec = 0;

#if 1
	tv3.tv_sec = 6;
	tv3.tv_usec = 0;

	tv4.tv_sec = 8;
	tv4.tv_usec = 0;

	tv5.tv_sec = 10;
	tv5.tv_usec = 0;

	tv6.tv_sec = 12;
	tv6.tv_usec = 0;

	tv7.tv_sec = 14;
	tv7.tv_usec = 0;
#endif

	event_add(&timeout1, &tv1);
	event_add(&timeout2, &tv2);
#if 1
	event_add(&timeout3, &tv3);
	event_add(&timeout4, &tv4);
	event_add(&timeout5, &tv5);
	event_add(&timeout6, &tv6);
	event_add(&timeout7, &tv7);
#endif
	event_dispatch();

	return 0;
}
