#ifndef __TCPTRANSPORT_H_INCLUDED_
#define __TCPTRANSPORT_H_INCLUDED_

#include "EventCallBack.h"

class TcpTransport : public EventCallBack
{
public:
	TcpTransport(int epollfd, int sockfd);
	~TcpTransport();
	void setEvents(int event);
	int getSockfd()const;
	void readEventHandle();
	void writeEventHandle();
	void timeOutEventHandle();
	void errorEventHandle();
private:
	int epollfd_;
	int connfd_;
	int events_;
};

#endif //__TCPTRANSPORT_H_INCLUDED_
