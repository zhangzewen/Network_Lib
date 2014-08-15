#ifndef _REACTOR_THREAD_H_INCLUDED__
#define _REACTOR_THREAD_H_INCLUDED__

#ifdef __cplusplus
extern "c" {
#endif

#include <pthread.h>
#include "list.h"

/**
 * the state of thread in the threadpool
 */
typedef enum{
	UNINIT,
	STARTING,
	STARTED,
	JOINING,
	STOPPING,
	STOPPED
}STATE;

/*
 * this is for the method of create a thread pool
 * CREATE_ALL, init the threads and create threads right now
 * CREATE_BY, init the threads and create thread whenever needed
 */
typedef enum{
	CREATE_ALL,
	CREATE_BY
}POOL_CREATE;

typedef struct thread_pool_st thread_pool_t;
typedef struct task_st task_t;
typedef struct thread_st thread_t;

struct task_st {
    void (*task_func)(void *arg);
    void *arg;
		struct list_head list;
};

struct thread_st {
	int thread_id;
	pthread_t pid;
	thread_pool_t *pool;
	task_t *current_task;
	STATE thread_state;
};

struct thread_pool_st{
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

thread_pool_t *thread_pool_init(int thread_num);
int thread_pool_add_task(thread_pool_t *pool, void (*task_func)(void *), void *arg);
void thread_pool_wait_for_done(thread_pool_t *pool);
void thread_pool_destroy(thread_pool_t *pool);
void get_worker();
void free_worker();
void get_connection();
void free_connection();

#ifdef __cplusplus
}
#endif

#endif
