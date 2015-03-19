#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_
class TcpServer
{
public:
	TcpServer() : listenfd_(-1), epollfd_(-1) {
	}
	~TcpServer();
	void Run();
private:
	int createSocketAndListen(bool nonblocking);
	int setNonblock(int fd);
	int listenfd_;
	int epollfd_;
};

#endif
