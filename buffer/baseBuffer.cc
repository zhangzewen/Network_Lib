#include "baseBuffer.h"

#include <string.h>
#include <glog/logging.h>


baseBuffer::baseBuffer() : pos_(NULL), start_(NULL), end_(NULL), last_(NULL),
  misAlign_(0), totalLen_(0), off_(0)
{
}

/**
  free buffer
*/
baseBuffer::~baseBuffer()
{
  if (start_) {
    free(start_);
  }
  pos_ = NULL;
  start_ = NULL;
  end_ = NULL;
  last_ = NULL;
  misAlign_ = 0;
  totalLen_ = 0;
  off_ = 0;
  LOG(INFO) << "free baseBuffer OK !";
}


/**
  expand n more bytes space to store the incoming data the length of the
  data is len
  @param len the lenght of data
 */
int baseBuffer::expand(size_t dataLen)
{
  size_t need = misAlign_ + off_ + dataLen;

  if (totalLen_ >= need) {
    return 0;
  }

  if (misAlign_ >= dataLen) {
    align();
  }else {
    void *newbuf;
    size_t length = totalLen_;

    if (length < 256) {
      length = 256;
    }   
    while (length < need) {
      length <<= 1;
    }   

    if (start_ != pos_ && misAlign_ != 0) {
      align();
    }   

    if ((newbuf = realloc(start_, length)) == NULL) {
      return -1; 
    }   

    start_ = pos_ = static_cast<char*>(newbuf);
    last_ = start_ + off_;
    totalLen_ = length;
    end_ = start_ + length;
  }

  return 0;
}

/**
  
*/
void baseBuffer::drain(size_t len)
{
  if(len >= off_) {
    pos_ = last_ = start_;
    misAlign_ = 0;
    off_ = 0;
    return ;
  }

  pos_ += len;
  misAlign_ += len;
  off_ -= len; 
}


/**
  memory align
*/
void baseBuffer::align()
{
  memmove(start_, pos_, off_);  
  pos_ = start_;
  last_ = pos_ + off_;
  misAlign_ = 0;
}

/**
  write to a discriptor, a wrapper function of system function read(),
  which should set O_NONBLOCK, this function write once  when it called

  @param fd file or socket discriptor
  @param len the length that will be write  
  @return -1, error happend, 1, write blocked or interrupt,
    0, EOF of file OR FIN of socket
    > 0, the lenght of readed data, maybe less than @param len
  @see read()
*/
int baseBuffer::Write(int fd, size_t len)
{
  int nwrite;
  if (len <= 0) {
    return -1;
  }

  if (len >= off_) {
    len = off_;
  }

  nwrite = write(fd, pos_, len);

  if (nwrite < 0) {
    if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR) {
      return -2;
    }
    return -1; 
  }

  if (nwrite == 0) { //  noting  writed
    return 0;
  }

  drain(nwrite); 
  return nwrite;
}

/**
  will write data as more as possible, but will not out of @param len
  Warning: this function can not tell wheter a error happend when writing
  you should check if there is a error happened

  @param fd file or socket discriptor
  @param len that will be write
*/
int baseBuffer::WriteN(int fd, size_t len)
{
  size_t  nleft;
  ssize_t nread;
 
  nleft = len;
  while (nleft > 0) {
    if ( (nread = Write(fd, nleft)) == -2) { //  not writeable or interrupt
      //  should not block
      break;
    } else if (nread == 0) { //  nothing writed
      break;
    } else if (nread == -1) {
      //  i feel bad for this
      break;
    }
    nleft -= nread;
  }
  return(len - nleft);
}

/**
  read from a discriptor, a wrapper function of system function read(),
  which should set O_NONBLOCK, this function read once  when it called

  @param fd file or socket discriptor
  @param len the lenght that will be read 
  @return -1, error happend, 1, read blocked or interrupt,
    0, EOF of file OR FIN of socket
    > 0, the lenght of readed data, maybe less than @param len
  @see read()
*/
int baseBuffer::Read(int fd, size_t len)
{
  int nread = 0;
  if (len < 0 || len > BUFFER_MAX_READ_) {
    len = BUFFER_MAX_READ_;
  }

  if (expand(len) == -1) {
    return -1;
  }

  nread = read(fd, last_, len);

  if (nread < 0) {
    if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR) {
      return -2;
    }
    return -1;
  }

  if (nread == 0) {
    return 0;
  }

  off_ += nread;
  last_ += nread;

  return nread;
}

/**
  will read data as more as possible, but will not out of @param len
  Warning: this function can not tell wheter a error happend when reading
  you should check if there is a error happened

  @param fd file or socket discriptor
  @param len will be read from @param fd
*/
int baseBuffer::ReadN(int fd, size_t len)
{
  size_t  nleft;
  ssize_t nread;
 
  nleft = len;
  while (nleft > 0) {
    if ( (nread = Read(fd, nleft)) == -2) { //  not readable or interrupt
      //  should not block
      break;
    } else if (nread == 0) { // get eof of file or FIN of socket
      break;
    } else if (nread == -1) {
      //  i feel bad for this
      break;
    }
    nleft -= nread;
  }
  return(len - nleft);
}

int baseBuffer::addBuffer(const char* buf, size_t len)
{
  return 0;
}

int baseBuffer::writeBuffer(char * buf, size_t len)
{
  return 0;
}
