#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#if 0
struct HashTable *hash_table_malloc(void *(*hash)(void *value),
                                  int (*data_compare)(void *dest_data, void *src_data),
                                  int (*key_compare)(void *dest_key, void *src_key))
{
	struct HashTable *head;
	head = (struct HashTable *)malloc(sizeof(struct HashTable));
	
	if (NULL == head) {
		exit(-1);
	}

	INIT_LIST_HEAD(&head->table_head);
	head->hash = hash;
	head->value_compare = data_compare;
	head->key_compare = key_compare;
	
	return head;
}
#endif

struct HashTable* hashtable_new(long int (*hash)(void *key),
																int (*key_compare)(void *dest_key, void *src_key),
																void (*key_free)(void **key),
																void (*value_free)(void **value))
{
	struct HashTable *new;
	new	= (struct HashTable *)calloc(sizeof(struct HashTable));
	if (NULL == new) {
		perror("hashtable calloc error!\n");
		return NULL;
	}

	new->mask = DEFAULTTABLESIZE - 1;
	new->total = DEFAULTTABLESIZE;
	new->hash = hash;
	new->key_compare = key_compare;
	new->key_free = key_free;
	new->value_free = value_free;
	return new;
}

struct Node *node_new(struct HashTable* hashtable)
{
	struct Node *node;
	
	node = (struct Node *)malloc(sizeof(struct Node));
	
	if(NULL == node) {
		return NULL;
	}

	INIT_LIST_HEAD(&node->list);
	node->data = NULL;
	node->key = NULL;
	node->hashtable = hashtable;
	
	return node;
}

void hashtable_free(struct HashTable **hashtable)
{
#if 0
	if(list_empty(&hash_table->table_head)){
		list_del(&hash_table->table_head);
	}else{
		struct Node *tmp, *ptr;
		list_for_each_entry_safe(ptr, tmp, &hash_table->table_head, node){
			node_free(ptr);
		}
		list_del(&hash_table->table_head);
	}

	hash_table->hash = NULL;
	hash_table->key_compare = NULL;
	
	free(hash_table);
	
	hash_table = NULL;
#endif
	if (NULL == *hashtable) {
		return ;
	}
	
	if (NULL != (*hashtable)->table) {
		int i = 0;
		struct Node *node = NULL;
		struct Node *tmp = NULL;
		struct list_head *head = NULL;
		for (i = 0; i < DEFAULTTABLESIZE; i++) {
			if ((*hashtable)->table[i] == NULL) {
				continue;
			}

			head = (*hashtable)->table[i];
			list_for_each_entry_safe(node, tmp, head, list) {
				list_del(&node->list);
				node_free(&node);
			}

			free(head);
			head = NULL;
		}	
	}	
	
	(*hashtable)->hash = NULL;
	(*hashtable)->key_compare = NULL
	(*hashtable)->key_free = NULL;
	(*hashtable)->value_free = NULL;
	
	free(*hashtable);
	*hashtable = NULL;
		
}



void node_free(struct Node **node)
{
	list_del(&(*node)->list);	
	
	if((*node)->key && NULL != (*node)->hashtable->key_free){
		(*node)->hashtable->key_free(&(*node)->key);
	}
	
	if((*node)->data && NULL != (*node)->hashtable->value_free) {
		(*node)->hashtable->value_free(&(*node)->data);
	}
	free(*node);
	*node = NULL;
}



struct Node *find_key_from_hash_table(struct HashTable *table, void *data)
{
	struct Node *tmp = NULL;
	struct Node *outer = NULL;
	
	if (NULL == table) {
		return NULL;
	}

	list_for_each_entry_safe(outer, tmp, &table->table_head, node) {
		if(table->key_compare(table->hash(data), outer->key) == 0)	{
			return outer;
		}
	}

	return NULL;
}

/*
 *struct Node *find_data_from_hash_table(struct HashTable *table, void *data)
 *@table, the Hash Table where zhe @data will be found from
 *@data, the element that will be found
 *return, NULL:found, Not NULL(struct Node *):not found
 */

struct Node *find_data_from_hash_table(struct HashTable *table, void *data)
{
	struct Node *node;
	if(NULL == table){
		return NULL;
	}
	
	node = find_key_from_hash_table(table, data);
	
	if(NULL == node) {
		return NULL;	
	}
	
	if(NULL != node) {
		
		if(list_empty(&node->child)){
			return node;
		}else{
			struct Node *inner, *tmp;
			list_for_each_entry_safe(inner, tmp, &node->child, node) {
				if(table->value_compare(inner->data, data) == 0) {
					return inner;
				}
			}
		}
	}
	
	return NULL;
}
/*
 *int add_data_to_hash_table(struct HashTable *table_head, void *data)
 *@table_head , the Hash Table
 *@data, data to add into Hash Table
 *return, -1:add error, 0:add ok, 1:record exists
 */



int add_record_to_hash_table(struct HashTable *table_head, void *data)
{
	struct Node *node;
	if(NULL == table_head)	{
		return -1;
	}	

	node = find_key_from_hash_table(table_head, data);

	if( NULL == node)	{
		node = node_malloc();

		if(NULL == node)	{
			return -1;
		}

		node->key = table_head->hash(data);

		node->data = data;

		node->hit = 0;

		list_add(&node->node, &table_head->table_head);

		return 0;
	}

	if(NULL != node){

		if(list_empty(&node->child)&& table_head->value_compare(node->data, data) != 0)	{
			struct Node *tmp;
			tmp = node_malloc();
			if(NULL == tmp)	{
				return -1;
			}

			tmp->key = node->key;
			tmp->data = node->data; 
			tmp->hit = node->hit;
			list_add(&tmp->node, &node->child);

			node->data = NULL;
			node->hit = 0;

			tmp = NULL;
			tmp = node_malloc();

			if(NULL == tmp)	{
				return -1;
			}

			tmp->key = node->key;
			tmp->data = data;
			tmp->hit = 0;

			list_add(&tmp->node, &node->child);
		
		}else if(list_empty(&node->child)&& table_head->value_compare(node->data, data) == 0) {
			node->hit++;
		}	

		if(!list_empty(&node->child)) {
			struct Node *tmp, *ptr;
			list_for_each_entry_safe(ptr, tmp, &node->child, node)	{
				if(table_head->value_compare(ptr->data, data) == 0) {
					ptr->hit++;
					return 0;
				}
			}

			tmp = NULL;
			tmp = node_malloc();

			if(NULL == tmp)	{
				return -1;
			}

			tmp->key = node->key;
			tmp->data = data;
			tmp->hit = 0;

			list_add(&tmp->node, &node->child);
		}
		return 0;

	}
	
	return -1;
}


/*
 *int del_record_to_hash_table(struct HashTable *table, void *data)
 *@table, the Hash Table
 *@data, a record that will del from Hash Table
 *return, -1: del error; 0:del ok!
 */


int del_record_to_hash_table(struct HashTable *table, void *data)
{
	struct Node *node;
	if(NULL == table){
		return -1;
	}

	node = find_data_from_hash_table(table, data);

	if(NULL == node) {
		return -1;	
	}

	if(NULL != node) {

		list_del(&node->node);
		node_free(node);
		return 0;
	}

	return -1;
}

