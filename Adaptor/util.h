#ifndef _ADAPTOR_UTIL_H_INCLUDED__
#define _ADAPTOR_UTIL_H_INCLUDED__

#include <vector>
#include <string>
namespace Util
{
#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))

int setNonBlock(int fd);

class StringTokenizer
{
public:
    typedef std::vector<std::string>::const_iterator TokenizerIter;
	StringTokenizer(const std::string& str, const std::string& token);
	~StringTokenizer();
	int countElements();
	std::string elementAt(int);
    std::string getNextElement();
private:
	std::vector<std::string> strs_;
    TokenizerIter current_;
};
}
#endif
