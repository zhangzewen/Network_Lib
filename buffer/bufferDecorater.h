#ifndef BUFFER_BUFFERDECORATER_H
#define BUFFER_BUFFERDECORATER_H


class bufferDecorater : public Buffer
{
public:
    int read(int fd, char* buf, int len) {
        buffer->read(fd, buf, len);
    }
    int write(int fd, char* buf, int len) {
        buffer->write(fd, buf, len);
    }
private:
    Buffer* buffer_;
};

#endif  // BUFFER_BUFFERDECORATER_H
