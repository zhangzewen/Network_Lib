#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <assert.h>

#include "http_epoll.h"
#include "event.h"
#include "list.h"
#include "RBTree.h"
#include "timer.h"
#include "event_base.h"

int terminate = 0;
struct eventop epollops = {
    .name = "epoll",
    .init = epoll_init,
    .add = epoll_add,
    .del = epoll_del,
    .dispatch = epoll_dispatch,
    .dealloc = epoll_dealloc,
};

static void event_queue_insert(struct event_base *, struct event *, int);

static void event_queue_remove(struct event_base *, struct event *, int);

static int event_haveevents(struct event_base *);

static void event_add_timer(struct event_base *, struct event *);

static void event_del_timer(struct event_base *, struct event *);

static int gettime(struct event_base *base, struct timeval *tp);

static int timeout_next(struct event_base *base, struct timeval **tv_p)
{
	struct timeval now;
	struct event *ev;
	struct timeval *tv = *tv_p;
	rbtree_node_t *tmp;

	if ((tmp = base->timeout.min(base->timeout.root)) == NULL) {
		*tv_p = NULL;
		return 0;
	}

	if (gettime(base, &now) == -1) {
		return -1;
	}

	ev =(struct event*)tmp->data;
	if (!timer_cmp(ev->ev_timeout, now)) {
		timer_reset(tv);
		return 0;
	}

	timer_sub(&ev->ev_timeout, &now , tv);
	return 0;
}

static int gettime(struct event_base *base, struct timeval *tp)
{
	if (base->tv_cache.tv_sec) {
		*tp = base->tv_cache;
		return 0;
	}
	return (gettimeofday(tp, NULL));
}

struct event_base *event_base_new(void)
{
	struct event_base *base;

	if ((base = calloc(1, sizeof(struct event_base))) == NULL) {
		fprintf(stderr, "%s: calloc\n", __func__);
	}

	gettime(base, &base->event_tv);
	rbtree_init(&base->timeout);
	INIT_LIST_HEAD(&base->eventqueue);
	INIT_LIST_HEAD(&base->activequeue);
	base->evbase = NULL;
	base->evsel = &epollops;
	base->evbase = base->evsel->init();

	if (base->evbase == NULL) {
		fprintf(stderr, "%s: no event mechanism available\n", __func__);
	}
	return (base);
}



static void event_process_active(struct event_base *base)
{
	struct event *ev;
	struct event *tmp;
	short ncalls;


	if(list_empty(&base->activequeue)) {
		return;
	}

	list_for_each_entry_safe(ev, tmp,&base->activequeue, active_list) {
		if (ev->ev_events & EV_PERSIST) {
			event_queue_remove(base, ev, EVLIST_ACTIVE);
		}else{
			event_del(ev);
		}

		ncalls = ev->ev_ncalls;
		ev->ev_pncalls = &ncalls;
		while(ncalls) {
			ncalls--;
			ev->ev_ncalls = ncalls;
			(*ev->callback)(ev);
		}

		if(list_empty(&base->activequeue)) {
			return;
		}
	}
}

#if 0
int event_dispatch(void)
{
	return (event_loop(0));
}
#endif

int event_base_dispatch(struct event_base *event_base)
{
	return (event_base_loop(event_base, 0));
}

const char *event_base_get_method(struct event_base *base)
{
	return (base->evsel->name);
}

void timeout_process(struct event_base *base)
{
	struct timeval now;
	struct event *ev;
	struct rbtree_node_st *tmp;


	if (base->timeout.empty(base->timeout.root)) {
		return ;
	}

	gettime(base, &now);

	while ((tmp = base->timeout.min(base->timeout.root))) {
		ev = (struct event *)tmp->data;
		if (timer_cmp(now, ev->ev_timeout) != 1) {
			break;
		}
		//event_del(ev);
		event_queue_remove(base, ev, EVLIST_TIMEOUT);
		ev->timeout = 1;
		event_active(ev, EV_TIMEOUT, 1);
	}
}

static void timeout_correct(struct event_base *base, struct timeval *tv)
{
	gettime(base, tv);
	//根据前面的分析可以知道event_ev应该小于tv_cache
	//如果tv < event_tv表明用户向前调整时间了，需要校正时间

	if (timer_cmp(*tv, base->event_tv)) {
		base->event_tv = *tv;
		return;
	}
}

int event_base_loop(struct event_base *base, int flags)
{
	struct eventop *evsel = base->evsel;
	struct epoll_loop *evbase = base->evbase;
	int res;
	struct timeval tv;
	struct timeval *tv_p;
	assert(flags == 0);
	base->tv_cache.tv_sec = 0;

	while(1) {
		timeout_correct(base, &tv);
		tv_p = &tv;

		if (!base->event_count_active ) {
			timeout_next(base, &tv_p);
		} else {
			timer_reset(&tv);
		}

		if (!event_haveevents(base)) {
			fprintf(stderr, "%s: no events registered.\n", __func__);
			return 1;
		}

		gettime(base, &base->event_tv);
		base->tv_cache.tv_sec = 0;
		res = evsel->dispatch(evbase, tv_p);

		if (res == -1) {
			return -1;
		}

		gettime(base, &base->tv_cache);
		timeout_process(base);
		if (base->event_count_active) {
			event_process_active(base);
		}
	}

	base->tv_cache.tv_sec = 0;
	fprintf(stderr, "%s: asked to terminate loop.\n", __func__);
	return 0;
}


int event_add(struct event *ev, const struct timeval *tv)
{
	struct event_base *base = ev->ev_base;
	struct eventop *evsel = base->evsel;
	struct epoll_loop *evbase =  base->evbase;
	int res = 0;

	if ((ev->ev_events & (EV_READ | EV_WRITE)) &&
		!(ev->ev_flags & (EVLIST_INSERTED | EVLIST_ACTIVE))) {
		res = evsel->add(evbase, ev);

		if (res != -1) {
			event_queue_insert(base, ev, EVLIST_INSERTED);
		}
	}

	if (res != -1 && tv != NULL) {
		struct timeval now;

		if (ev->ev_flags & EVLIST_TIMEOUT) {
			event_queue_remove(base, ev, EVLIST_TIMEOUT);
		}

		if ((ev->ev_flags & EVLIST_ACTIVE) && (ev->ev_res & EV_TIMEOUT)) {

			if (ev->ev_ncalls && ev->ev_pncalls) {
				*ev->ev_pncalls = 0;
			}

			event_queue_remove(base, ev, EVLIST_ACTIVE);
		}

		gettime(base, &now);
		timer_add(&now, tv, &ev->ev_timeout);
		event_queue_insert(base, ev, EVLIST_TIMEOUT);
		ev->timeout_set = 1;
	}

	return res;
}

void event_set(struct event_base *base, struct event *ev, int fd, short events,
	void (*callback)(struct event *ev), void *arg, char *name)
{
	ev->ev_base =base;
	ev->callback = callback;
	ev->ev_arg = arg;
	ev->ev_fd = fd;
	ev->ev_events = events;
	ev->ev_res = 0;
	ev->ev_flags = EVLIST_INIT;
	ev->ev_ncalls = 0;
	ev->ev_pncalls = NULL;
	ev->name = name;
	ev->timeout = 0;
	ev->timeout_set = 0;
	ev->ready = 0;
	ev->active = 0;
	INIT_LIST_HEAD(&ev->event_list);
	INIT_LIST_HEAD(&ev->active_list);
}


int event_del(struct event *ev)
{
	struct event_base *base;
	struct eventop *evsel;
	struct epoll_loop *evbase;

	if (ev->ev_base == NULL) {
		return -1;
	}

	base = ev->ev_base;
	evsel = base->evsel;
	evbase = base->evbase;

	if (ev->ev_ncalls && ev->ev_pncalls) {
		*ev->ev_pncalls = 0;
	}


	if (ev->ev_flags & EVLIST_ACTIVE) {
		event_queue_remove(base, ev, EVLIST_ACTIVE);
	}

	if (ev->ev_flags & EVLIST_INSERTED) {
		event_queue_remove(base, ev, EVLIST_INSERTED);
		return (evsel->del(evbase, ev));
	}

	if (ev->ev_flags & EVLIST_TIMEOUT) {
		event_queue_remove(base, ev, EVLIST_TIMEOUT);
	}

	return 0;
}

void event_active(struct event *ev, int res, short ncalls)
{
	if (ev->ev_flags & EVLIST_ACTIVE) {
		ev->ev_res |= res;
		return ;
	}

	ev->ev_res = res;
	ev->ev_ncalls = ncalls;
	ev->ev_pncalls = NULL;
	event_queue_insert(ev->ev_base, ev, EVLIST_ACTIVE);
}


void event_queue_insert(struct event_base *base, struct event *ev, int queue)
{
	if (ev->ev_flags & queue) {
		if (queue & EVLIST_ACTIVE) {
			return ;
		}
	}

	if (~ev->ev_flags & EVLIST_INTERNAL) {
		base->event_count++;
	}

	ev->ev_flags |= queue;

	switch(queue) {
		case EVLIST_INSERTED:
			list_add_tail(&ev->event_list, &base->eventqueue);
			break;
		case EVLIST_ACTIVE:
			base->event_count_active++;
			list_add_tail(&ev->active_list, &base->activequeue);
			break;
		case EVLIST_TIMEOUT:
			event_add_timer(base, ev);
			break;
		default:
			fprintf(stderr, "%s: unknown queue %x\n", __func__, queue);
	}
}



void event_queue_remove(struct event_base *base, struct event *ev, int queue)
{
	if (!(ev->ev_flags & queue)) {
		fprintf(stderr, "%s: %p(fd %d) not no queue %x\n", __func__, ev, ev->ev_fd, queue);
	}

	if (~ev->ev_flags & EVLIST_INTERNAL) {
		base->event_count--;
	}

	ev->ev_flags &= ~queue;

	switch (queue) {
		case EVLIST_INSERTED:
			list_del(&ev->event_list);
			break;
		case EVLIST_ACTIVE:
			base->event_count_active--;
			list_del(&ev->active_list);
			break;
		case EVLIST_TIMEOUT:
			event_del_timer(base, ev);
			break;

		default:
			fprintf(stderr, "%s: unknown queue %x\n", __func__, queue);
	}
}



int event_haveevents(struct event_base *base)
{
	return base->event_count > 0;
}

static void event_add_timer(struct event_base *base, struct event *ev)
{
	uintptr_t key = 0;

	if (!timer_isset(&ev->ev_timeout)) {
		return ;
	}

	if (ev->timeout_set) {
		event_del_timer(base, ev);
	}

	key = ev->ev_timeout.tv_sec * 1000 + ev->ev_timeout.tv_usec / 1000;
	base->timeout.root = base->timeout.insert(key, (void*)ev, base->timeout.root);
}

static void event_del_timer(struct event_base* base, struct event *ev)
{
	uintptr_t key = 0;

	if (!timer_isset(&ev->ev_timeout)) {
		return ;
	}

	key = ev->ev_timeout.tv_sec * 1000 + ev->ev_timeout.tv_usec / 1000;
	base->timeout.root = base->timeout.erase(key, base->timeout.root);
	return;
}


void event_free(struct event **ev, int flags)
{
	struct event *tmp;
	tmp = *ev;

	if (NULL == tmp) {
		return ;
	}

	tmp->name = NULL;
	tmp->ev_pncalls = NULL;
	tmp->callback = NULL;

	if (flags) {
		free(tmp->ev_arg);
	}

	tmp->ev_arg = NULL;
	free(tmp);
	tmp = NULL;
	*ev = tmp;
}


void event_base_free(struct event_base **base)
{
	(*base)->evsel->dealloc(&(*base)->evbase);
	(*base)->evbase = NULL;
	(*base)->evsel->name = NULL;
	(*base)->evsel->init = NULL;
	(*base)->evsel->add = NULL;
	(*base)->evsel->del = NULL;
	(*base)->evsel->dispatch = NULL;
	(*base)->evsel->dealloc = NULL;
	rbtree_free(&(*base)->timeout.root);
	free((*base));
	base = NULL;
}

