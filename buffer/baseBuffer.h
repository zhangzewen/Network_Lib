#ifndef BUFFER_BASEBUFFER_H
#define BUFFER_BASEBUFFER_H_

#include <stdlib.h>
#include "buffer.h"

class baseBuffer : public Buffer
{
 public:
  baseBuffer();
  ~baseBuffer();
  int write(int fd, int len);
  int writeN(int fd, int len);
  int read(int fd, int len);
  int readN(int fd, int len);
  int addBuffer(const char* buf, int len);
  bool empty() const {
    return off_ == 0;
  }
  int size() const {
    return off_;
  }

private:
  void drain(int len);
  void align();
  int expand(int len);
  const int BUFFER_MAX_READ_ = 1024;
  char *pos_;
  char *start_;
  char *end_;
  char *last_;
  int misAlign_;
  int totalLen_;
  int off_;
};

#endif  //  BUFFER_BASEBUFFER_H
