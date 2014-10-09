#ifndef __NETWORKLIB_UTIL_URL_HPP_INCLUDED__
#define __NETWORKLIB_UTIL_URL_HPP_INCLUDED__
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
	string host_;
	string path_;
	std::map<string, string> params_;
};

}
}

#endif
