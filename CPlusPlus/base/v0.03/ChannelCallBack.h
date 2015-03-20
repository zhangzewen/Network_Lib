#ifndef __CHANNELCALLBACK_H_INCLUDED__
#define __CHANNELCALLBACK_H_INCLUDED__

class ChannelCallBack
{
public:
	virtual void CallBack(int fd) = 0;
};

#endif
