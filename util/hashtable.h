#ifndef __HASHTABLE_H_INCLUDED
#define __HASHTABLE_H_INCLUDED

#include "list.h"

#define DEFAULTTABLESIZE  1024

struct HashTable{
	struct Node **table;
	int mask;
	int total;
	long int (*hash)(void *key);
	int (*key_compare)(void *dest_key, void *src_data);
	void  (*key_free)(void **key);
	void (*value_free)(void **value);
};

struct Node{
	struct list_head list;
	void *data;
	void *key;
	struct HashTable *HashTable;
	int hit;
};


struct HashTable *hashtable_new(long int (*hash)(void *key),
																int (*key_compare)(void *dest_key, void *src_key),
																void (*key_free)(void **key),	
																void (*value_free)(void **value));

void hashtable_free(struct HashTable **hashtable);
struct Node *node_new();
void node_free(struct Node **node);

struct Node *find_data_from_hash_table(struct HashTable *table, void *key);
int add_record_to_hash_table(struct HashTable *table_head, void *key, void *value);
int del_record_to_hash_table(struct HashTable *table, void *key);
int update_record_to_hash_table(struct HashTable *table, void *key, void *data)
#endif

