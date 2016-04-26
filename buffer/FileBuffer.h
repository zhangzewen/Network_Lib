#ifndef BUFFER_FILEBUFFER_H
#define BUFFER_FILEBUFFER_H

#include "BufferDecorater.h"


class FileBuffer : public BufferDecorater
{
public:
    FileBuffer(Buffer* buf);
    FileBuffer(Buffer* buf, const std::string& path);
    int read(int fd, char* buf, int len);
    int write(int fd, char* buf, int len);
    bool open();
    void close();
private:
    int fd_;
    std::string path_;
    Buffer* buffer_;

}


#endif  // BUFFER_FILEBUFFER_H
