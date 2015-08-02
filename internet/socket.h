#ifndef _ADAPTOR_SOCKET_H_INCLUDED_
#define _ADAPTOR_SOCKET_H_INCLUDED_

class Socket
{
public:
  Socket(int fd);
  ~Socket();
private:
  int fd_;
};
#endif //_ADAPTOR_SOCKET_H_INCLUDED_
