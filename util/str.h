#ifndef _UTIL_STR_H_INCLUDED__
#define _UTIL_STR_H_INCLUDED__

#include <stdarg.h>
#include <stdlib.h>

typedef struct str_st str_t;

struct str_st {
	size_t len;
	char *data;
};


str_t *str_dup(const char *ptr, size_t len);

#define str(ptr) { sizeof(ptr) - 1, (char *)ptr} 
#define str_null {0, NULL}
#define str_compare(str1, str2) strcasecmp(str1->data, str2->data) 
#define tolower(c)	(char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define toupper(c)	(char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

#endif
