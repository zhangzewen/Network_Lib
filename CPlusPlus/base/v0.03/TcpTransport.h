#ifndef __TCPTRANSPORT_H_INCLUDED_
#define __TCPTRANSPORT_H_INCLUDED_

class TcpTransport
{
public:
	TcpTransport(int epollfd, int sockfd);
	~TcpTransport();
	void callBack();
	int registerEvent();
	void setEvents(int event);
	int getSockfd()const;
private:
	int epollfd_;
	int connfd_;
	int events_;
};

#endif //__TCPTRANSPORT_H_INCLUDED_
