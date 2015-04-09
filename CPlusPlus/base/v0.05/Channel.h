#ifndef _CHANNEL_H_INCLUDED__
#define _CHANNEL_H_INCLUDED__

#include "ChannelCallBack.h"
#include <memory>
class Channel : public std::enable_shared_from_this<Channel>
{
public:
	Channel(int epollfd, int sockfd);
	~Channel();
	void setCallBack(std::shared_ptr<ChannelCallBack> callback);
	void handleEvent();
	int getSockfd()const;
	int setEvents(int event);
	int clearEvents(int event);
	int registerEvent();
private:
	int epollfd_;
	int fd_;
	int events_;
	std::shared_ptr<ChannelCallBack> callBack_;
};

#endif

