#ifndef BUFFER_FILEBUFFER_H
#define BUFFER_FILEBUFFER_H

#include "BufferDecorater.h"


class FileBuffer : public BufferDecorater
{
public:
    int read(int fd, char* buf, int len);
    int write(int fd, char* buf, int len);
private:
    int fd_;
    File* fp_;
    std::string path_;
    Buffer* buffer_;

}


#endif  // BUFFER_FILEBUFFER_H
