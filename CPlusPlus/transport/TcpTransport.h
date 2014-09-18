#ifndef _NETWORKLIB_TRANSPORT_TCPTRANSPORT_HPP_INCLUDED_
#define _NETWORKLIB_TRANSPORT_TCPTRANSPORT_HPP_INCLUDED_

#include "ITransport.hpp"
namespace NetworkLib{
namespace transport{

class TcpTransport : public ITranport
{
public:
private:
	int fd;
	std::string address_;
	int port_;

};
}
}

#endif