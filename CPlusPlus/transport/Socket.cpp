
#include "Socket.hpp"
void Socket::close()
{
	fd_.close();
}

std::string Socket::getHost()
{

}

std::string Socket::getPeerAddress()
{
}

std::string Socket::getPeerHost()
{

}

std::string Socket::getPeerPort()
{
}

bool setConnTimeout(struct timveval)
{

}

void Socket::setHost(std::string host)
{
}

void Socket::setPort(int port)
{
	port_ = port;
}

void Socket::setKeepalived(bool on)
{

}

void Socket::setKeepalivedTimeout(struct timveval)
{

}

bool Socket::setLinger(bool on)
{

}

bool Socket::setNoDely(bool on)
{
}

bool Socket::setRecvTimeout(struct timeval)
{

}

bool Socket::setSendTimeout(struct timeval)
{
}


#endif
