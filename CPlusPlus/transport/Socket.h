#ifndef _NETWORKLIB_TRANSPORT_SOCKET_HPP_INCLUDED_
#define _NETWORKLIB_TRANSPORT_SOCKET_HPP_INCLUDED_
#include <string.h>
#include <iostream>
namespace NetworkLib{
namespace transport{

class Socket
{
public:
	Socket(){}
	Socket(std::string host, int port);
	int Listen(int backlog);
	int Bind();
	~Socket();
	Socket(int fd);
	int Close();
	std::string getHost() const;
	void setHost(std::string host);
	int getPort() const;
	void setPort(int port);
	bool setNonBlocking();
private:
	std::string host_;
	int port_;
	int fd_;
};
}
}
#endif
