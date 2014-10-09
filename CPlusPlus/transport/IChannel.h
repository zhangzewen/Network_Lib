#ifndef __NETWORKLIB_TRANSPORT_ICHANNEL_H_INCLUDED_
#define __NETWORKLIB_TRANSPORT_ICHANNEL_H_INCLUDED_

namespace NetworLib{
namespace transport{
class IChannel
{
public:
	// when Readable
	virtual ReadEventCallBack();
	// when Writeable
	virtual WriteEventCallBack();
	// when error
	virtual ErrorEventCallBack();
	// when timeout
	virtual TimeoutEventCallBack();
};
}
}

#endif
