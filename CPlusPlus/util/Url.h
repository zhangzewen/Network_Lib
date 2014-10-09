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
	Url() : parser_(NULL), url_(NULL) {}
	Url(http_parser* tmp);
	~Url();
	void Parse(const std::string& urlStr);
private:
	std::string host_;
	std::string path_;
	std::map<std::string, std::string> params_;
	http_parser* parser_;
	http_parser_url* url_;
};

}
}

#endif
