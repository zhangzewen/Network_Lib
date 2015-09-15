#ifndef __EVENTCALLBACK_H_INCLUDED__
#define __EVENTCALLBACK_H_INCLUDED__

class EventCallBack
{
public:
	virtual void readEventHandle() = 0;
	virtual void writeEventHandle() = 0;
	virtual void timeOutEventHandle() = 0;
	virtual void errorEventHandle() = 0;
};

#endif //__EVENTCALLBACK_H_INCLUDED__

