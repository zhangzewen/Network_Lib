#ifndef __TCPTRANSPORT_H_INCLUDED_
#define __TCPTRANSPORT_H_INCLUDED_

class TcpTransport
{
public:
	TcpTransport(int epollfd, int sockfd);
	~TcpTransport();
private:
	int epollfd_;
	int socketfd_;
};

#endif //__TCPTRANSPORT_H_INCLUDED_
