#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "Thread.h"
#include "list.h"

static void *thread_routine(void *arg);

thread_pool_t *thread_pool_create(int thread_num) {
    int i;
    thread_pool_t *pool;
    pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));

    pthread_mutex_init(&pool->queue_mutex, NULL);
    pthread_cond_init(&pool->queue_cond_ready, NULL);

    pool->queued_task_count = 0;
    pool->waiting_and_running_task_count = 0;
    pool->shutdown = 0;
    pool->thread_count = thread_num;
    pool->thread_id = (pthread_t *) malloc(thread_num * sizeof(pthread_t));
		INIT_LIST_HEAD(&pool->thread_task);

    for(i = 0; i < thread_num; ++i)
        pthread_create(&pool->thread_id[i], NULL, thread_routine, pool);

    return pool;
}

void thread_pool_add_task(thread_pool_t *pool, void (*task_func)(void *), void *arg) {
    thread_task_t *task = (thread_task_t *)malloc(sizeof(thread_task_t));
    task->task_func = task_func;
    task->arg = arg;
		INIT_LIST_HEAD(&task->list);

    pthread_mutex_lock(&pool->queue_mutex);
    pool->queued_task_count++;
		list_add_tail(&task->list, &pool->thread_task);
    pthread_mutex_unlock(&pool->queue_mutex);

    pthread_mutex_lock(&pool->count_mutex);
    pool->waiting_and_running_task_count++;
    pthread_mutex_unlock(&pool->count_mutex);

    pthread_cond_signal(&pool->queue_cond_ready);
}

int thread_pool_get_task_count(thread_pool_t *pool)
{
    volatile int count;
    pthread_mutex_lock(&pool->count_mutex);
    count = pool->waiting_and_running_task_count;
    pthread_mutex_unlock(&pool->count_mutex);
    return count;
}

void thread_pool_wait_for_done(thread_pool_t *pool)
{
    while(thread_pool_get_task_count(pool) > 0)
        usleep(100000);
}

void thread_pool_destroy(thread_pool_t *pool) {
    thread_task_t *task;
		thread_task_t *tmp;
    int i;
    if(pool->shutdown)
        return;
    pthread_mutex_lock(&pool->queue_mutex);
    pool->shutdown = 1;
    pthread_mutex_unlock(&pool->queue_mutex);

    pthread_cond_broadcast(&pool->queue_cond_ready);
    for(i = 0; i < pool->thread_count; ++i)
        pthread_join(pool->thread_id[i], NULL);

    free(pool->thread_id);
		list_for_each_entry_safe(task, tmp, &pool->thread_task, list) {
			list_del(&task->list);
			free(task);
		}
		
    pthread_mutex_destroy(&pool->queue_mutex);
    pthread_cond_destroy(&pool->queue_cond_ready);
    free(pool);
}

static void *thread_routine(void *arg) {
    thread_pool_t *pool;
    thread_task_t *task;
    pool = (thread_pool_t *)arg;
    while(1) {
        pthread_mutex_lock(&pool->queue_mutex);
        while(pool->queued_task_count == 0 && !pool->shutdown)
            pthread_cond_wait(&pool->queue_cond_ready, &pool->queue_mutex);

        if(pool->shutdown){
            pthread_mutex_unlock(&pool->queue_mutex);
            pthread_exit(NULL);
        }

        pool->queued_task_count--;
				task = list_first_entry(&pool->thread_task, struct task, list);
				list_del(&task->list);
        pthread_mutex_unlock(&pool->queue_mutex);

        (*task->task_func)(task->arg);
        free(task);
        task = NULL;

        pthread_mutex_lock(&pool->count_mutex);
        pool->waiting_and_running_task_count--;
        pthread_mutex_unlock(&pool->count_mutex);
    }
}
