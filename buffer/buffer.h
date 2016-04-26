#ifndef BUFFER_BUFFER_H_
#define BUFFER_BUFFER_H_

class Buffer
{
public:
    virtual int read(int fd, char* buf, int len) = 0;
    virtual int write(int fd, char* buf, int len) = 0;
};
#endif  //  BUFFER_BUFFER_H_
