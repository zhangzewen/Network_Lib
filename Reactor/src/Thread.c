#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include "Thread.h"
#include "list.h"
#include "event_base.h"
#include "event.h"

static void *thread_routine(void *arg);
static void thread_free(thread_t *thread_id);
static pthread_mutex_t init_lock;
static pthread_cond_t init_cond;
static int last_thread = -1;
static int num_threads = 0;
static int init_count = 0;
struct conn_item *queue_pop(struct conn_queue *);
void queue_push(struct conn_queue *, struct conn_item *);

//==============item===============================
struct conn_item *item_new()
{
	struct conn_item *new;
	new = calloc(1, sizeof(struct conn_item));
	if (NULL == new) {
		return NULL;
	}
	INIT_LIST_HEAD(&new->list);
	return new;
}

void item_free(struct conn_item **item)
{
	free((*item));
	*item = NULL;
}

//================thread=======================================

void thread_event_process(struct event *ev);
void register_thread_initialized(void)
{
	pthread_mutex_lock(&init_lock);
	init_count++;
	pthread_cond_signal(&init_cond);
	pthread_mutex_unlock(&init_lock);
}

int thread_setup(thread_t *thread)
{
	if (NULL == thread) {
		return -1;
	}

	int fds[2];
	if (pipe(fds)) {
		fprintf(stderr, "can not create notify pipe");
		return -1;
	}	

	thread->notify_receive_fd = fds[0];
	thread->notify_send_fd = fds[1];
	
	thread->base = event_base_new();
	if (NULL == thread->base) {
		fprintf(stderr, "can not alloc event_base");
		return -1;
	}

	event_set(thread->base, &thread->notify_event, thread->notify_receive_fd,
		 EV_READ | EV_PERSIST, thread_event_process, thread, NULL);
	if (event_add(&thread->notify_event, 0) == -1) {
		fprintf(stderr, "can not monitor event notify pipe \n");
		exit(1);
	}
	
	INIT_LIST_HEAD(&thread->queue.list);
	pthread_mutex_init(&thread->queue.lock, NULL);
	return 0;
}


static void conn_queue_free(struct conn_queue *queue)
{
	struct conn_item *item;
	struct conn_item *tmp;
	if (NULL == queue) {
		return;
	}
	list_for_each_entry_safe(item, tmp, &queue->list, list) {
		list_del(&item->list);
		item_free(&item);
	}

	pthread_mutex_destroy(&queue->lock);
	
}
static void thread_free(thread_t *thread_id)
{
	event_base_free(&thread_id->base);
	free(thread_id->current_task);
	close(thread_id->notify_receive_fd);
	close(thread_id->notify_send_fd);
	conn_queue_free(&thread_id->queue);
}

void thread_event_process(struct event *ev)
{
	thread_t *thread = (thread_t *)ev->ev_arg;
	struct conn_item *item;
	char buf[1];

	if (read(ev->ev_fd, buf, 1) != 1) {
		fprintf(stderr, "can not read from libevent pipe\n");
		return ;
	}

	switch(buf[0]) {
		case 'c':
			item = queue_pop(&thread->queue);

			if (NULL != item) {
				thread->current_task->func(item->sfd, item->events, thread->base, thread->current_task);
				item_free(&item);
			}
			break;
		default:
			fprintf(stderr, "error!\n");
			break;
	}
}




void wait_for_thread_registration(int nthreads)
{
	while(init_count < nthreads) {
		pthread_cond_wait(&init_cond, &init_lock);
	}
}

void queue_push(struct conn_queue *queue, struct conn_item *item)
{
	pthread_mutex_lock(&queue->lock);
	list_add(&item->list, &queue->list);
	pthread_mutex_unlock(&queue->lock);
}

struct conn_item *queue_pop(struct conn_queue *queue)
{
	struct conn_item *item = NULL;
	pthread_mutex_lock(&queue->lock);
	if (!list_empty(&queue->list)) {
		item = list_first_entry(&queue->list, struct conn_item, list);
	}
	if (item) {
		list_del(&item->list);
	}
	pthread_mutex_unlock(&queue->lock);
	return item;
	
}


//================pool=============================
void dispatch_conn_new(int fd, short event_flags, thread_pool_t *pool)
{
	struct conn_item *item = NULL;
	char buf[1] = {0};

	item = item_new();
	int tid = (last_thread + 1) % num_threads;
	thread_t *thread = pool->threads + tid;
	last_thread = tid;
	item->sfd = fd;
	item->events = event_flags;
	queue_push(&thread->queue, item);
	buf[0] = 'c';
	if (write(thread->notify_send_fd, buf, 1) != 1) {
		perror("Writing to thread notify pipe");
	}
}

thread_pool_t *thread_pool_create(int thread_num)
{
    int i;
    thread_pool_t *pool;
    pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));
    pthread_mutex_init(&pool->queue_mutex, NULL);
    pthread_cond_init(&pool->queue_cond_ready, NULL);
		pthread_mutex_init(&init_lock, NULL);
		pthread_cond_init(&init_cond, NULL);

    pool->queued_task_count = 0;
    pool->waiting_and_running_task_count = 0;
    pool->shutdown = 0;
    pool->thread_count = thread_num;
    pool->threads = (thread_t *)calloc(thread_num, sizeof(struct thread_st));
		if (NULL == pool->threads) {
			pthread_cond_destroy(&pool->queue_cond_ready);
			pthread_mutex_destroy(&pool->queue_mutex);
			pthread_mutex_destroy(&init_lock);
			pthread_cond_destroy(&init_cond);
			free(pool);
			return NULL;
		}
		
		for (i = 0; i < thread_num; i++) {
			pool->threads[i].thread_id = i;
			pool->threads[i].pool = pool;
			thread_setup(&pool->threads[i]);
		}     

		INIT_LIST_HEAD(&pool->thread_task);

    for(i = 0; i < thread_num; ++i) {
			pthread_create(&pool->threads[i].pid, NULL, thread_routine, &pool->threads[i]);
		}


		pthread_mutex_lock(&init_lock);
		wait_for_thread_registration(thread_num);
		pthread_mutex_unlock(&init_lock);
		num_threads = thread_num;

		return pool;
}
void thread_pool_add_task(thread_pool_t *pool, int (*task_func)(int , short, struct event_base *,void *), void *arg) {
    thread_task_t *task = (thread_task_t *)malloc(sizeof(thread_task_t));
    task->func = task_func;
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
        pthread_join(pool->threads[i].pid, NULL);

    for(i = 0; i < pool->thread_count; ++i) {
    	thread_free((pool->threads + i));
		}

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
	thread_t *thread;
	thread = (thread_t*)arg;
	pool = thread->pool;
	register_thread_initialized();
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
	task->thread = thread;
	thread->current_task = task;
	pthread_mutex_unlock(&pool->queue_mutex);

	event_base_loop(thread->base, 0);
#if 0
	event_base_free(&thread->base);
	free(thread->current_task);
#endif

	pthread_mutex_lock(&pool->count_mutex);
	pool->waiting_and_running_task_count--;
	pthread_mutex_unlock(&pool->count_mutex);
	return NULL;
}

