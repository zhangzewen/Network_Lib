#ifndef INTERNET_SOCKET_H
#define INTERNET_SOCKET_H

class Socket
{
public:
  Socket(int fd);
  ~Socket();
private:
  int fd_;
};
#endif //INTERNET_SOCKET_H
