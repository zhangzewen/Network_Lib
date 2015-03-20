#ifndef _CHANNEL_H_INCLUDED__
#define _CHANNEL_H_INCLUDED__

class Channel
{
public:
	Channel(int epollfd, int sockfd);
	~Channel();
	void setCallBack(ChannelCallBack* callback);
	void CallBack(int sockfd);
private:
	int epollfd_;
	int fd_;
	ChannelCallBack* callBack_;
};

#endif

