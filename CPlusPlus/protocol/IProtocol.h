#ifndef __NETWORKLIB_UTIL_IPROTOCOL_H_INCLUDED_
#define __NETWORKLIB_UTIL_IPROTOCOL_H_INCLUDED_
#include "exceptions.h"
using namespace NetworkLib::concurrency;

namespace NetworkLib {
namespace Util{

class IProtocol {
public:
	IProtocol();
	~IProtocol();
	virtual void dataRecived();
	virtual void ConnectionMade();
	virtual void ConnectionLost(exceptions& e);
	virtual void MakeConnection();
};
}
}

#endif
