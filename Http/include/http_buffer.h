#ifndef _EVBUFFER_H_INCLUDED__
#define _EVBUFFER_H_INCLUDED__

#include <stdio.h>
#include <stdarg.h>


struct http_buffer{
	char *pos;
	char *start;
	char *end;
	char *last;
	
	size_t misalign;
	size_t totallen;
	size_t off;
};


struct buffer_chain_st {
	struct http_buffer *buf;
	struct buffer_chain_st *next;
};


struct http_buffer *buffer_new(void);

void buffer_free(struct http_buffer *);

int buffer_expand(struct http_buffer *, size_t);

void buffer_drain(struct http_buffer *, size_t);

int buffer_write(struct http_buffer *, int);

int buffer_read(struct http_buffer *, int, int);

#endif
