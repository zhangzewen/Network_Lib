#ifndef _REACTOR_THREAD_H_INCLUDED__
#define _REACTOR_THREAD_H_INCLUDED__

#ifdef __cplusplus
extern "c" {
#endif

#include <pthread.h>
#include "list.h"

typedef struct task {
    void (*task_func)(void *arg);
    void *arg;
		struct list_head list;
}thread_task_t;

typedef struct {
    pthread_mutex_t queue_mutex;
    pthread_mutex_t count_mutex;
    pthread_cond_t queue_cond_ready;
		struct list_head thread_task;
    int queued_task_count; 
    int shutdown;
    pthread_t *thread_id;
    int thread_count;
    int waiting_and_running_task_count;
}thread_pool_t;

thread_pool_t *thread_pool_create(int thread_num);
void thread_pool_add_task(thread_pool_t *pool, void (*task_func)(void *), void *arg);
void thread_pool_wait_for_done(thread_pool_t *pool);
void thread_pool_destroy(thread_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif
