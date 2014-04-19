#include "buf.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

struct buffer * buffer_new(void)
{
	struct buffer *buffer;
	buffer = (struct buffer *)malloc(sizeof(struct buffer));
	return buffer;
}


void buffer_free(struct buffer *buffer)
{
	if (buffer->orig_buffer != NULL) {
		free(buffer->orig_buffer);
	}
	free(buffer);
}


#define SWAP(x, y) do {\
	(x)->buffer = (y)->buffer; \
	(x)->orig_buffer = (y)->orig_buffer; \
	(x)->misalign = (y)->misalign; \
	(x)->totallen = (y)->totallen;	\
	(x)->off = (y)->off;	\
}while (0)

int buffer_add_buffer(struct buffer *outbuf, struct buffer *inbuf)
{
	int res;
	
	if(outbuf->off == 0) {
		struct buffer tmp;
		size_t oldoff = inbuf->off;
		
		SWAP(&tmp, outbuf);
		SWAP(outbuf, inbuf);
		SWAP(inbuf, &tmp);
		
		if (inbuf->off != oldoff && inbuf->cb != NULL) {
			(*inbuf->cb)(inbuf, oldoff, inbuf->off, inbuf->cbarg);
		}
		
		if (oldoff && outbuf->cb != NULL) {
			(*outbuf->cb)(outbuf, 0, oldoff, outbuf->cbarg);
		}
		
		return 0;
	}
	
	res = buffer_add(outbuf, inbuf->buffer, inbuf->off);

	if (res == 0) {
		buffer_drain(inbuf, inbuf->off);
	}
	return res;
}



int buffer_remove(struct buffer *buf, void *data, size_t datlen)
{
	size_t nread = datlen;
	if (nread >= buf->off) {
		nread = buf->off;
	}

	memcpy(data, buf->buffer, nread);
	buffer_drain(buf, nread);
	return nread;
}

static void buffer_align(struct buffer *buf)
{
	memmove(buf->orig_buffer, buf->buffer, buf->off);
	buf->buffer = buf->orig_buffer;
	buf->misalign = 0;
}

/*Expands the available space in the event buffer to at least datlen*/

int buffer_expand(struct buffer *buf, size_t datlen)
{
	size_t need = buf->misalign + buf->off + datlen;
	
	/*If we can fit all the data, the we don't have to do anything*/
	if (buf->totallen >= need) {
		return 0;
	}

	//If the misalignment fulfills out data needs, we just force an 
	//alignment to happen, Afterwards ,we have enough space.
	if (buf->misalign >= datlen) {
		buffer_align(buf);
	}else {
		void *newbuf;
		size_t length = buf->totallen;

		if (length < 256) {
			length = 16;
		}
		while (length < need) {
			length <<= 1;
		}

		if (buf->orig_buffer != buf->buffer) {
			buffer_align(buf);
		}
		if ((newbuf = realloc(buf->buffer, length)) == NULL) {
			return -1;
		}

		buf->orig_buffer = buf->buffer = newbuf;
		buf->totallen = length;
	}

	return 0;
}


int buffer_add(struct buffer *buf, const void *data, size_t datlen)
{
	size_t need = buf->misalign + buf->off + datlen;
	size_t oldoff = buf->off;
	
	if (buf->totallen < need) {
		if (buffer_expand(buf, datlen) == -1) {
			return -1;
		}
	}

	memcpy(buf->buffer + buf->off, data, datlen);
	buf->off += datlen;
	if (datlen && buf->cb != NULL) {
		(*buf->cb)(buf, oldoff, buf->off, buf->cbarg);
	}
	return 0;
}

void buffer_drain(struct buffer *buf, size_t len)
{
	size_t oldoff = buf->off;
	
	if(len >= buf->off) {
		buf->off = 0;
		buf->buffer = buf->orig_buffer;
		buf->misalign = 0;
		goto done;
	}

	buf->buffer += len;
	buf->misalign += len;
	
	buf->off -= len;
done:
	// Tell someone about changes in this buffer
	if (buf->off != oldoff &&  buf->cb != NULL) {
		(*buf->cb)(buf, oldoff, buf->off, buf->cbarg);
	}
}


/*
 *Reads data from a file descriptor into a buffer.
 */

#define EVBUFFER_MAX_READ 4096

int buffer_read(struct buffer *buf, int fd, int howmuch)
{
	unsigned char *p;
	size_t oldoff = buf->off;
	int n = EVBUFFER_MAX_READ;

	if (howmuch < 0 || howmuch > n) {
		howmuch = n;
	}

	/*If we don't have FIONREAD,we might waste some space here*/

	if (buffer_expand(buf, howmuch) == -1) {
		return -1;
	}

	p = buf->buffer + buf->off;

	n = read(fd, p, howmuch);

	if (n == -1) {
		return -1;
	}

	if (n == 0) {
		return 0;
	}

	buf->off += n;

	/*Tell somone abot changes in this buffer*/

	if (buf->off != oldoff && buf->cb != NULL) {
		(*buf->cb)(buf, oldoff, buf->off, buf->cbarg);
	}

	return n;
}


int buffer_write(struct buffer *buffer, int fd)
{
	int n;
	
	n = write(fd, buffer->buffer, buffer->off);

	if (n == -1) {
		return -1;
	}

	if (n == 0) {
		return 0;
	}

	buffer_drain(buffer, n);

	return n;
}

unsigned char *buffer_find(struct buffer *buffer, const unsigned char *what, size_t len)
{
	unsigned char *search = buffer->buffer;
	unsigned char *end = search + buffer->off;

	unsigned char *p;
	
	while(search < end && (p = memchr(search, *what, end - search)) != NULL) {
		if (p + len > end) {
			break;
		}

		if (memcmp(p, what, len) == 0) {
			return p;
		}

		search = p + 1;
	}
	return NULL;
}

