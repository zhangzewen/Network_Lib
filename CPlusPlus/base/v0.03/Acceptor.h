#ifndef _ACCEPTOR_H_INCLUDED__
#define _ACCEPTOR_H_INCLUDED__

#include "AcceptorCallBack.h"

class Acceptor : public ChannelCallBack
{
public:
	Acceptor() {}
	~Acceptor() {}
	void setAccptorCallBack(AcceptorCallBack* callback);
	int createSocketAndListen(bool nonblocking);
	int setEvents(int event);
	int registerEvent();
	void channelCallBack();
private:
	int listenfd_;
	int epollfd_;
	int events_;
	AcceptorCallBack* callBack_;

};
#endif
