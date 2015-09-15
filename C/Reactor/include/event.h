#ifndef __EVENT_H_INCLUDED__
#define __EVENT_H_INCLUDED__

#include <sys/time.h>
#include "event_base.h"

struct event_base *event_base_new();

int event_dispatch();

int event_base_dispatch(struct event_base *);

void event_set(struct event_base *base, struct event *ev, int fd,
	short events, void (*callback)(struct event *ev), void *arg, char *name);

int event_base_loop(struct event_base *, int flags);

int event_add(struct event *ev, const struct timeval *timeout);

int event_del(struct event *ev);

void event_active(struct event *, int res, short ncalls);

void event_free(struct event **ev, int flags);

void event_base_free(struct event_base **base);
#endif
