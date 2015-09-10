#ifndef BUFFER_BASEBUFFER_H
#define BUFFER_BASEBUFFER_H_

#include <stdlib.h>

struct baseBuffer
{
 public:
  baseBuffer();
  ~baseBuffer();
  int Write(int fd, size_t len);
  int WriteN(int fd, size_t len);
  int Read(int fd, size_t len);
  int ReadN(int fd, size_t len);
  void drain(size_t len);
  void align();
  int expand(size_t len);
  int addBuffer(const char* buf, size_t len);
  bool empty() const {
    return off_ == 0;
  }
  int size() const {
    return off_;
  }

  static const int BUFFER_MAX_READ_ = 1024;
  char *pos_;
  char *start_;
  char *end_;
  char *last_;
  size_t misAlign_;
  size_t totalLen_;
  size_t off_;
};

#endif  //  BUFFER_BASEBUFFER_H
