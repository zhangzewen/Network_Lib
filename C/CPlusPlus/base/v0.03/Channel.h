#ifndef _CHANNEL_H_INCLUDED__
#define _CHANNEL_H_INCLUDED__

//#include <boost/function.hpp>
#include <functional>
class Channel
{
public:
    typedef std::function<void ()> callback;
	Channel(int epollfd, int sockfd);
	~Channel();
	void setReadCallBack(const callback& cb);
	void handleEvent();
	int getSockfd()const;
	int setEvents(int event);
	int registerEvent();
private:
	int epollfd_;
	int fd_;
	int events_;
//	ChannelCallBack* callBack_;
    callback readcallback;
};

#endif

