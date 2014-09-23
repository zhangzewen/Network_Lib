#ifndef __NETWORKLIB_TRANSPORT_ICHANNEL_H_INCLUDED_
#define __NETWORKLIB_TRANSPORT_ICHANNEL_H_INCLUDED_

class IChannel
{
public:
	virtual ReadEventCallBack();
	virtual WriteEventCallBack();
	virtual ErrorEventCallBack();
};

#endif
