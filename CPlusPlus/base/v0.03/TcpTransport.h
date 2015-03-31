#ifndef __TCPTRANSPORT_H_INCLUDED_
#define __TCPTRANSPORT_H_INCLUDED_

class TcpTransport
{
public:
	TcpTransport(int epollfd, int sockfd);
	~TcpTransport();
private:
};

#endif //__TCPTRANSPORT_H_INCLUDED_
