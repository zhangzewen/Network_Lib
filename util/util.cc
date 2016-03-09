#include <stdio.h>
#include <fcntl.h>
#include "util.h"

int setNonBlock(int fd)
{
	int ret;
	if ((ret = fcntl(fd, F_GETFL)) < 0) {
		return -1;
	}
	ret |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, ret) < 0) {
		return -1;
	}
	return 0;
}

