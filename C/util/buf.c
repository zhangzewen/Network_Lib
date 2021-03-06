#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "buf.h"

http_buf_t *buffer_new(void)
{
	http_buf_t *buffer;
	buffer = (http_buf_t *)malloc(sizeof(http_buf_t));
	return buffer;
}


void buffer_free(http_buf_t *buffer)
{
	if (buffer->start != NULL) {
		free(buffer->start);
	}
	free(buffer);
}


static void buffer_align(http_buf_t *buf)
{
	memmove(buf->start, buf->pos, buf->off);
	buf->pos = buf->start;
	buf->misalign = 0;
	buf->last = buf->start + buf->off;
	
}


int buffer_expand(http_buf_t *buf, size_t datlen)
{
	size_t need = buf->misalign + buf->off + datlen;
	
	if (buf->totallen >= need) {
		return 0;
	}

	if (buf->misalign >= datlen) {
		buffer_align(buf);
	}else {
		void *newbuf;
		size_t length = buf->totallen;

		if (length < 256) {
			length = 256;
		}
		while (length < need) {
			length <<= 1;
		}

		if (buf->start != buf->pos && buf->misalign != 0) {
			buffer_align(buf);
		}
		int pos_off = buf->pos - buf->start;
		if ((newbuf = realloc(buf->start, length)) == NULL) {
			return -1;
		}
#if 0
		buf->start = buf->pos = newbuf;
		buf->last = buf->start;
		buf->totallen = length;
		buf->end = buf->start + length;
#endif
		
		if (buf->misalign == 0) {
			buf->start = newbuf;
			buf->pos = buf->start +pos_off; 
		} else {
			buf->start = newbuf;
			buf->pos = buf->start;
		}
		buf->last = buf->start + buf->off;
		buf->end = buf->start + length;
		buf->totallen = length;
		
	}

	return 0;
}

void buffer_drain(http_buf_t *buf, size_t len)
{
	
	if(len >= buf->off) {
		buf->pos = buf->last = buf->start;
		buf->misalign = 0;
		buf->off = 0;
		return ;
	}

	buf->pos += len;
	buf->misalign += len;
	
	buf->off -= len;
}


#define BUFFER_MAX_READ 4096

int buffer_read(http_buf_t *buf, int fd, int howmuch)
{
	char *p;
	int n = BUFFER_MAX_READ;

	if (howmuch < 0 || howmuch > n) {
		howmuch = n;
	}

	/*If we don't have FIONREAD,we might waste some space here*/

	if (buffer_expand(buf, howmuch) == -1) {
		return -1;
	}

	p = buf->last;

	n = read(fd, p, howmuch);

	if (n == -1) {
		return -1;
	}

	if (n == 0) {
		return 0;
	}

	buf->off += n;
	buf->last += n;

	return n;
}


int buffer_write(http_buf_t *buffer, int fd)
{
	int n;
	
	n = write(fd, buffer->pos, buffer->off);

	if (n == -1) {
		return -1;
	}

	if (n == 0) {
		return 0;
	}

	buffer_drain(buffer, n);

	return n;
}

