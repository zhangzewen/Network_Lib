#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "list.h"

kv *kv_new(void *key, void *value)
{
	kv *new = NULL;
	new = (kv*)calloc(1, sizeof(kv));
	if (NULL == new) {
		return NULL;
	}

	new->key = key;
	new->value = value;

	INIT_LIST_HEAD(&new->list);
	return new;
}




