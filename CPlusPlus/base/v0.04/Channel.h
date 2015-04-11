#ifndef _CHANNEL_H_INCLUDED__
#define _CHANNEL_H_INCLUDED__

#include "EventCallBack.h"
#include "Dispatcher.h"
class Channel
{
public:
	Channel(int fd);
	Channel(int fd, Dispatcher*);
	Channel();
	~Channel();
	void setCallBack(EventCallBack* callback);
	void handleEvent(int activeEvents);
	int getFd()const;
	void setFd(int);
	void setDispatcher(Dispatcher*);
	int getEvents()const;
	int setEvents(int event);
	int registerEvent(int event);
	int unregisterEvent(int event);
private:
	int fd_;
	int events_;
	EventCallBack* callBack_;
	Dispatcher* base_;
};

#endif //_CHANNEL_H_INCLUDED__

