#ifndef _NETWORKLIB_TRANSPORT_CHANNEL_H_INCLUDED_
#define _NETWORKLIB_TRANSPORT_CHANNEL_H_INCLUDED_

namespace NetworkLib {
namespace Transport{

class Channel
{
public:
	Channel(int epollfd, int sockfd);
	~Channel();
	void setCallBack(IChannel* channelCallBack);
	void setRevent(int revent);
	void handleEvent();
	int getEvents();
	int getSockfd();
private:
	Channel(Channel&); //不能拷贝赋值
	int event_;
	int revent_;
	int sockfd_;
	int epollfd_;
};

}
}

#endif //_NETWORKLIB_TRANSPORT_CHANNEL_H_INCLUDED_
