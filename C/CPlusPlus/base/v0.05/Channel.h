#ifndef _CHANNEL_H_INCLUDED__
#define _CHANNEL_H_INCLUDED__

#include "EventCallBack.h"
#include "Dispatcher.h"
#include <memory>
#include <functional>

class Dispatcher;

class Channel : public std::enable_shared_from_this<Channel>
{
	public:
		typedef std::function<void()> callbackFunctor;
		Channel(int fd);
		Channel(int fd, std::weak_ptr<Dispatcher>);
		Channel();
		virtual ~Channel();
		void setReadCallBack(const callbackFunctor& cb) {
            readCallBack_ = cb;
        }
		void setWriteCallBack(const callbackFunctor& cb) {
            writeCallBack_ = cb;
        }
		void setErrorCallBack(const callbackFunctor& cb) {
            errorCallBack_ = cb;
        }
		void setTimeOutCallBack(const callbackFunctor& cb) {
            TimeOutCallBack_ = cb;
        }
		int getFd()const;
		void setFd(int);
		void setDispatcher(std::weak_ptr<Dispatcher>);
		int getEvents()const;
		int setEvents(int event);
		int registerEvent(int event);
		int unRegisterEvent(int event);
		void handleEvent(int activeEvents);
	private:
		int fd_;
		int events_;
		std::shared_ptr<EventCallBack> callBack_;
		std::weak_ptr<Dispatcher> base_;
		callbackFunctor readCallBack_;
		callbackFunctor writeCallBack_;
		callbackFunctor errorCallBack_;
		callbackFunctor TimeOutCallBack_;
};

#endif //_CHANNEL_H_INCLUDED__

