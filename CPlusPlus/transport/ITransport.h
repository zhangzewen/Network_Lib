#ifndef __NETWORKLIB_TRANSPORT_ITRANSPORT_HPP_INCLUDED_
#define __NETWORKLIB_TRANSPORT_ITRANSPORT_HPP_INCLUDED_

namespace NetworkLib{
namespace Transport{
class ITransport
{
public:
	virtual uint32_t write(uint8_t* buf, uint32_t len) = 0;
	virtual uint32_t read(uint8_t* buf, uint32_t len) = 0;
	virtual std::string getPeer();
	virtual std::string getHost();
	virtual bool isOpen();
	virtual bool open();
	virtual int peek();
};
}
}

#endif

