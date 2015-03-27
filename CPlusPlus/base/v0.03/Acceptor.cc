#include "Acceptor.h"

int Acceptor::createSocketAndListen(bool nonblocking)
{
	int listenfd;
	socklen_t len;
	struct sockaddr_in srvaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		std::cerr << "socket error!" << std::endl;
		return -1;
	}
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(8080);
	len = sizeof(srvaddr);
	if (bind(listenfd, (struct sockaddr*)&srvaddr, len) < 0) {
		std::cerr << "socket bind error!" << std::endl;
		return -1;
	}
	int ret = listen(listenfd, 100);
	if (listenfd < 0) {
		std::cerr << "listen error" << std::cout;
		return -1;
	}
	if (nonblocking) {
		if (setNonblock(listenfd) < 0) {
			std::cerr << "listenfd set nonblocking error!" << std::endl;
			return -1;
		}
	}
	return listenfd;
}

void Acceptor::setAcceptorCallBack(AcceptorCallBack* callback)
{
	callBack_ = callback;
}
