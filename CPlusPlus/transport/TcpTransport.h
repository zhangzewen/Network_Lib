#ifndef _NETWORKLIB_TRANSPORT_TCPTRANSPORT_HPP_INCLUDED_
#define _NETWORKLIB_TRANSPORT_TCPTRANSPORT_HPP_INCLUDED_

#include "ITransport.hpp"
#include "Buffer.h"

using namespace NetworkLib::Util;
namespace NetworkLib{
namespace transport{

class TcpTransport : public ITranport
{
public:
	uint32_t write(uint8_t* buf, uint32_t len);
	uint32_t read(uin8_t* buf, uint32_t len);
	std::string getPeer();
	std::string getHost();
	bool isOpen();
	bool open();
	int peek();
private:
	Buffer* buf_;
	Socket* socket_;

};
}
}

#endif
