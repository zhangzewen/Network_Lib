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

std::string to_string(int num)
{
  char buf[256] = {0};
  sprintf(buf, "%d", num);
  return buf;
}
std::string to_string(long num)
{
  char buf[256] = {0};
  sprintf(buf, "%ld", num);
  return buf;
}
std::string to_string(long long num)
{
  char buf[256] = {0};
  sprintf(buf, "%lld", num);
  return buf;
}
std::string to_string(unsigned num)
{
  char buf[256] = {0};
  sprintf(buf, "%u", num);
  return buf;
}
std::string to_string(unsigned long num)
{
  char buf[256] = {0};
  sprintf(buf, "%lu", num);
  return buf;
}
std::string to_string(unsigned long long num)
{
  char buf[256] = {0};
  sprintf(buf, "%llu", num);
  return buf;
}

std::string to_string(float num)
{
  char buf[256] = {0};
  sprintf(buf, "%f", num);
  return buf;
}
std::string to_string(double num)
{
  char buf[256] = {0};
  sprintf(buf, "%f", num);
  return buf;
}
std::string to_string(long double num)
{
  char buf[256] = {0};
  sprintf(buf, "%Lf", num);
  return buf;
}
