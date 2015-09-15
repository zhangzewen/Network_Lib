#ifndef _REACTOR_THREAD_H_INCLUDED__
#define _REACTOR_THREAD_H_INCLUDED__

#include <pthread.h>
#include "list.h"
#include "event_base.h"

typedef struct thread_pool_st thread_pool_t;
typedef struct thread_st thread_t;

struct conn_item {
  int sfd;
  int events;
	struct list_head list;
};

struct conn_queue{
	struct list_head list;
  pthread_mutex_t lock; 
};

typedef struct task {
    int (*func)(int , short , struct event_base *, void *);
    void *arg;
		thread_t *thread;
		struct list_head list;
}thread_task_t;

struct thread_st {
	int thread_id;
	pthread_t pid;
	struct event_base *base;
	struct event notify_event;
	int notify_receive_fd;
	int notify_send_fd;
	thread_pool_t *pool;
	thread_task_t *current_task;
	struct conn_queue queue;
};


struct thread_pool_st {
    pthread_mutex_t queue_mutex;
    pthread_mutex_t count_mutex;
    pthread_cond_t queue_cond_ready;
		struct list_head thread_task;
    int queued_task_count;
    int shutdown; 
    thread_t *threads;
    int thread_count; 
    int waiting_and_running_task_count;
};

thread_pool_t *thread_pool_create(int thread_num);
void thread_pool_add_task(thread_pool_t *pool, 
	int (*task_func)(int , short , struct event_base *, void *), void *arg);
void thread_pool_wait_for_done(thread_pool_t *pool);
void thread_pool_destroy(thread_pool_t *pool);
void ServerListening(struct event *ev);

#endif
