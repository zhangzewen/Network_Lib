#ifndef _CHANNEL_H_INCLUDED__
#define _CHANNEL_H_INCLUDED__

#include "EventCallBack.h"
#include "Dispatcher.h"
#include <memory>

class Dispatcher;

class Channel : public std::enable_shared_from_this<Channel>
{
public:
	Channel(int fd);
	Channel(int fd, std::shared_ptr<Dispatcher>);
	Channel();
	virtual ~Channel();
	void setCallBack(std::shared_ptr<EventCallBack> callback);
	void handleEvent(int activeEvents);
	int getFd()const;
	void setFd(int);
	void setDispatcher(std::shared_ptr<Dispatcher>);
	int getEvents()const;
	int setEvents(int event);
	int registerEvent(int event);
	int unRegisterEvent(int event);
private:
	int fd_;
	int events_;
	std::shared_ptr<EventCallBack> callBack_;
	std::weak_ptr<Dispatcher> base_;
};

#endif //_CHANNEL_H_INCLUDED__

