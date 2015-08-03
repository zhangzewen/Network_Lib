#ifndef INTERNET_SOCKET_H_
#define INTERNET_SOCKET_H_

class Socket
{
public:
  Socket(int fd);
  ~Socket();
private:
  int fd_;
};
#endif //INTERNET_SOCKET_H
