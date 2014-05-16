#ifndef __DICT_H_INCLUDED
#define __DICT_H_INCLUDED

#define TABLE_SIZE 1024


#include "list.h"
typedef struct kv_st {
	void *key;
	void *value;
	struct list_head list;
}kv;


typedef struct dict_st {
	kv **table;
	unsigned long size;
	unsigned long mask;
	unsigned long int (*hash)(void *key);
	int (*key_free)(void **key);
	int (*data_free)(void **data);
	int (*key_compare)(void *dest_key, void *src_key);
}dict;



#endif
