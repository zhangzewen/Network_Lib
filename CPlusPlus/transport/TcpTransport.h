#ifndef _NETWORKLIB_TRANSPORT_TCPTRANSPORT_HPP_INCLUDED_
#define _NETWORKLIB_TRANSPORT_TCPTRANSPORT_HPP_INCLUDED_

#include <boost/shared_ptr.hpp>
#include "ITransport.hpp"
#include "Buffer.h"

using namespace NetworkLib::Util;
namespace NetworkLib{
namespace transport{

class TcpTransport : public ITranport
{
public:
	TcpTransport();
	TcpTransport(boost::shared_ptr<Socket>& socket) : socket_(socket) {
	}
private:
	boost::shared_ptr<Socket> socket_;
	boost::shared_ptr<Channel> channel_;
};

}
}

#endif
