#ifndef __NETWORKLIB_TRANSPORT_ITRANSPORT_HPP_INCLUDED_
#define __NETWORKLIB_TRANSPORT_ITRANSPORT_HPP_INCLUDED_

namespace NetworkLib{
namespace Transport{
class ITransport
{
public:
	virtual uint32_t write(int fd, int8_t* buf, uint32_t len) = 0;
	virtual uint32_t read(int fd, int8_t* buf, uint32_t len) = 0;
	virtual uint32_t isConnect();
	virtual uint32_t isOpen();
};
}
}

#endif

