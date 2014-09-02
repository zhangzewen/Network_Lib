#include "Buffer.hpp"
#include "../config.h"

ssize_t Buffer::read(int fd, uint8_t* buf, uint32_t howmuch)
{
	char *p = NULL;

	if (howmuch < 0 || howmuch > 4096) {
		howmuch = 4096;
	}

	if (expend(howmuch) == -1) {
		return -1;
	}

	char *p = last_;
	n =read(fd, p, howmuch);
	if (n >= 0) {
		off_ += n;
		last_ += n;
		return n;
	}

	if (errno == EAGAIN || errno == EINTR) {
		return NL_AGAIN;
	}
	return NL_ERROR; // now it must be wrong!
}

ssize_t Buffer::write(int fd, uint8_t*, uint32_t len)
{
	int n;
	n = write(fd, pos_, off);
	if (n >= 0) {
		drain(n);
		return n;
	}

	if (errno == EAGAIN || errno == EINTR) {
		return NL_AGAIN;
	}
	return NL_ERROR; // now it must be wrong!
}


int Buffer::align()
{
	memmove(start_, pos_, off_);
	pos_ = start_;
	missalign_ = 0;
	last_ = pos_ + off_;
}

int expend(size_t datalen)
{
	size_t need = missalign_ + off_ + datalen;

	if (totallen_ > need) {
		return 0;
	} else {
		void *newbuf;
		size_t length = totallen_;
		if (length < 256) {
			length = 256;
		}
		while(length < need && length <= BUFFER_MAX_LEN) {
			length = length << 1;
		}
		if (pos_ != start_ && missalign_ != 0) {
			align();
		}
		if ((newbuf = (uint8_t*)calloc(start_, length)) == NULL){
			return -1;
		}
		start_ = pos_ = newbuf;
		totallen_ = lenght;
		last_ = pos_ + off_;
		end_ = start_ + length;
		return 0;
	}
	return -1;
}

void Buffer:drain(size_t len)
{
	if (len > off_) {
		pos_ = last_ = start_;
		missalign_ = 0;
		off_ = 0;
		return;
	}

	pos_ += len;
	missalign_ += len;
	off_ -= len;
}
