#include "Socket.h"
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Config.h"

namespace NetworkLib{
namespace transport{

Socket::Socket(std::string host, int port)
{
	host_ = host;
	port_ = port;
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
}

int Socket::Bind()
{
	struct sockaddr_in sockaddr;
	socklen_t len = sizeof(sockaddr);
	inet_aton(host_.c_str(), &sockaddr.sin_addr);
	return bind(fd_, (struct sockaddr*)&sockaddr, len);
}

int Socket::Listen(int backlog)
{
	return listen(fd_, backlog);
}

int Socket::Accept()
{
	struct sockaddr sockaddr;
	socklen_t len;
	int sfd = accept(fd_, (struct sockaddr*)&sockaddr, &len);
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


int Socket::Close()
{
	close(fd_);
}

int Socket::setNonBlocking()
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
}
}
