#ifndef _HTTPBUFFER_H_INCLUDED__
#define _HTTPBUFFER_H_INCLUDED__
#ifdef __cplusplus
extern "c" {
#endif

#include <stdlib.h>
typedef struct http_buffer_st http_buf_t;

struct http_buffer_st{
	char *pos;
	char *start;
	char *end;
	char *last;
	
	size_t misalign;
	size_t totallen;
	size_t off;
};


http_buf_t *buffer_new(void);

void buffer_free(http_buf_t *);

int buffer_expand(http_buf_t *, size_t);

void buffer_drain(http_buf_t *, size_t);

int buffer_write(http_buf_t *, int);

int buffer_read(http_buf_t *, int, int);
#ifdef __cplusplus
}
#endif

#endif
