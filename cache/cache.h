#ifndef _CACHE_H_INCLUDED__
#define _CACHE_H_INCLUDED__

#include "hashtable.h"
#include "event_base.h"

typedef struct cache_node_st cache_node_t;


struct cache_node_st {
	struct Node_st *node;
	struct event ev;
	struct timeval last_visit;
	struct timeval expire_time;
	struct cache_st *cache;
};

struct cache_st {
	struct event_base *base;
	struct HashTable_st *hashtable;
	struct list_head lru_list;
};


cache_node_t *cache_node_create(struct cache_st *cache, void *key, void *value, unsigned int expire)
{
	cache_node_t *new = NULL;
	new = (cache_node_t *)calloc(1, sizeof(struct cache_node_t));
	
	if (NULL == new) {
		return NULL;
	}

	new->node = node_new(cache->table, key, value);
	if (NULL == new->node) {
		free(new);
		return NULL;
	}

	new->node->data = (void *)new;
	new->expire_time.tv_sec = expire;
	new->expire_time.tv_usec = 0;	
}
#endif
