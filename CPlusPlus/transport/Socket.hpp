#ifndef _NETWORKLIB_TRANSPORT_SOCKET_HPP_INCLUDED_
#define _NETWORKLIB_TRANSPORT_SOCKET_HPP_INCLUDED_

class Socket
{
public:
	Socket(){}
	Socket(std::string host, int port);
	~Socket();
	Socket(int fd);
	close();
	getHost();
	getPeerAddress();
	getPeerHost();
	getPeerPort();
	getSocketInfo();
	isOpen();
	open();
	peek();
	read();
	write();
	setConnTimeout();
	setHost();
	setLinger();
	setNoDelay();
	setPort();
	setRecvTimeout();
	setSendTimeout();
private:
	int connTieout_;
	std::string host_;
	bool lingerOn_;
	int lingerVal_;
	std::peerAddress_;
	std::peerHost_;
	int peerProt_;
	int RecvTimeout_;
	int sendTimeout_;
	bool KeepAliveOn_;
	int KeepAliveTimeout_;
	int port_;
};

#endif
