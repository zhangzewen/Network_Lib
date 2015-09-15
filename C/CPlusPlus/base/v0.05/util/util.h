#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__
namespace Util
{
int daemonize(int nochdir, int noclose);
std::string lstrip(const std::string& str, std::string decollator = "\f\v\t\n\r ");
std::string rstrip(const std::string& str, std::string decollator = "\f\v\t\n\r ");
std::string strip(std::string& str, std::string decollator = "\f\v\t\n\r ");
void split(const std::string&, const std::string& decollator, std::vector<std::string>& array);
}
#endif
