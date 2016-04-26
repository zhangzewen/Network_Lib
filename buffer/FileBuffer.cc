#include "FileBuffer.h"

FileBuffer::FileBuffer(Buffer* buf) : fd_(-1), buffer_(buf)
{
}

FileBuffer::FileBuffer(Buffer* buf, const std::string& path) : fd_(-1),
    path_(path), buffer_(buf)
{
}

bool FileBuffer::open()
{
    fd_ = open(path_.c_str(), O_RDWR | O_CREAT);
    if (fd_ < 0) {
        return false;
    }
    return true;
}

void FileBuffer::close()
{
    close(fd);
}

