#ifndef _NETWORKLIB_TRANSPORT_SOCKET_HPP_INCLUDED_
#define _NETWORKLIB_TRANSPORT_SOCKET_HPP_INCLUDED_
#include <string.h>
#include <iostream>


class Socket
{
public:
	Socket(){}
	Socket(std::string host, int port);
	int Listen(int backlog);
	int Accept();
	int Bind();
	~Socket();
	Socket(int fd);
	int Close();
	std::string getHost() const;
	void setHost(std::string host);
#if 0
	std::string getPeerAddress() const;
	void setPeerAddress(std::string peerAddress_);
	std::string getPeerHost() const;
	void setPeerHost(std::string peerHost);
	int getPeerPort() const;
	void setPeerPort(int peerPort);
#endif
	int getPort() const;
	void setPort(int port);
	int setNonBlocking();
private:
	std::string host_;
	int port_;
	int fd_;
#if 0
	std::peerHost_;
	int peerProt_;
	std::peerAddress_;
#endif
};
#endif
