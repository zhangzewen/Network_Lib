#include "socket.h"
#include <unistd.h>

Socket::Socket(int fd) : fd_(fd)
{
}

Socket::~Socket()
{
  close(fd_);
}
