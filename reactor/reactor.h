// Copyright [2015] <Zhang Zewen>
//
//
#include "callBackHandle.h"
#include "event.h"
#include "epoll.h"
#include "dispatcher.h"
#include "RBTree.h"
#include "timer.h"


#ifndef REACTOR_REACTOR_H_
#define REACTOR_REACTOR_H_

#define REACTOR_EV_READ     0x01
#define REACTOR_EV_WRITE    0x02
#define REACTOR_EV_TIMEOUT  0x04
#define REACTOR_EV_ERROR    0x08

#define POLL_NOT_WAIT_TYPE          0x01
#define POLL_TIMEOUT_WAIT_TYPE      0x02
#define POLL_INDEFINITELY_WAIT_TYPE 0x04



#endif  // REACTOR_REACTOR_H_
