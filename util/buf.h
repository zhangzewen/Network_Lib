#ifndef _EVBUFFER_H_INCLUDED__
#define _EVBUFFER_H_INCLUDED__

#include <stdio.h>
#include <stdarg.h>
#define EVBUFFER_READ 0X01
#define EVBUFFER_WRITE	0X02
#define EVBUFFER_EOF	0X10
#define EVBUFFER_ERROR	0X20
#define EVBUFFER_TIMEOUT	0X40
/*
*libevent 的缓冲是一个连续的内存区域，其处理数据的方式（写数据和读数据）更像一个队列操作方式：从后面写入，从前读出。
*
*/
struct buffer{
	unsigned char *buffer;//buffer指向有效的数据的内存区域
	unsigned char *orig_buffer; // 指向由malloc分配的连续内存区域
	
	size_t misalign;//表示buffer相对于orig_buffer的偏移
	size_t totallen;//拜师orig_buffer指向的内存区域的大小
	size_t off;//表示有效的数据的长度

	void (*cb)(struct buffer *, size_t, size_t, void *);
	void *cbarg;
};

#define BUFFER_LENGTH(X) (X)->off
#define BUFFER_DATA(X) (X)->buffer
#define BUFFER_INPUT(X) (X)->input
#define BUFFER_OUTPUT(X) (X)->output

struct buffer *buffer_new(void);

void buffer_free(struct buffer *);

int buffer_expand(struct buffer *, size_t);

int buffer_add(struct buffer *, const void *, size_t);

int buffer_remove(struct buffer *, void *, size_t);


int buffer_add_buffer(struct buffer *, struct buffer *);


void buffer_drain(struct buffer *, size_t);

int buffer_write(struct buffer *, int);

int buffer_read(struct buffer *, int, int);

unsigned char  *buffer_find(struct buffer *, const unsigned char *, size_t);

#endif
