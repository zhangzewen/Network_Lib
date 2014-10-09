#ifndef __NETWORKLIB_UTIL_URL_HPP_INCLUDED__
#define __NETWORKLIB_UTIL_URL_HPP_INCLUDED__

#include <map>
#include <string>
#include "http_parser.h"
namespace NetworkLib {
namespace Util {

class Url
{
public:
	Url();
	~Url();
	void split();
	void parse();
private:
	std::string host_;
	std::string path_;
	std::map<std::string, std::string> params_;
};

}
}

#endif
