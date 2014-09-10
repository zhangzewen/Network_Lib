
#include "Socket.hpp"
#include "Config.hpp"


Socket::Socket(std::string host, int port)
{
	host_ = host;
	port_ = port;
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	return fd_;
}

int Socket::bind()
{
	struct sockaddr_in sockaddr;
	socklen_t len = sizeof(sockaddr);
	inet_aton(host_.c_str()), &sockaddr);
	return bind(fd, &sockaddr, len);
}

int Socket::listen(int backlog)
{
	return listen(fd_, backlog);
}

int Socket::accept()
{
	struct sockaddr sockaddr;
	socklen_t len;
	int sfd = accept(fd, &sockaddr, &len);
	if (sfd == -1) {
		if (errno == EAGAIN) {
			return NL_AGAIN;
		}
		return NL_ERROR;
	}
	return sfd;
}

std::string Socket::getHost() const
{
	return host_;
}
void Socket::setHost(std::string host)
{
	host_ = host;
}

int Socket::getPort() const
{
	return port_;
}

void Socket::setPort(int port)
{
	port_ = port;
}

#if 0
void Socket::setPeerAddress(std::string peerAddress)
{
	peerAddress_ = peerAddress;
}

std::string Socket::getPeerAddress() const
{
	return peerAddress_;
}

void Socket::setPeerHost(std::string peerHost)
{
	peerHost_ = peerHost;
}

int Socket::getPeerHost() const
{
	return peerHost_;

}

void Socket::setPeerPort()
{
	peerPort_ = peerPort;
}

std::string Socket::getPeerPort() const
{
	return peerProt_;
}
#endif

void Socket::close()
{
	fd_.close();
}

int setNonBlocking()
{
	int flag = 0;
	if ((flag = fcntl(fd_, F_GETFD)) == -1) {
		return -1;
	}

	if ((fcntl(fd_, F_SETFD, flag | O_NONBLOCK)) == -1) {
		return -1;
	}
	return 0;
}
#endif
