#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <string>
#include <vector>

int daemonize(int nochdir, int noclose);
int setNonBlock(int fd);
std::string lstrip(const std::string& str, const std::string& decollator = "\f\v\t\n\r ");
//std::string lstrip(const std::string& str, const char* decollator = "\f\v\t\n\r ");
std::string rstrip(const std::string& str, const std::string& decollator = "\f\v\t\n\r ");
//std::string rstrip(const std::string& str, const char* decollator = "\f\v\t\n\r ");
std::string strip(std::string& str, const std::string& decollator = "\f\v\t\n\r ");
//std::string strip(std::string& str, const char* decollator = "\f\v\t\n\r ");
void split(const std::string&, const std::string& decollator, std::vector<std::string>& array);

std::string short2str(short int num);
std::string ushort2str(unsigned short int num);
std::string uint2str(unsigned int num);
std::string int2str(int num);
std::string ulong2str(unsigned long int num);
std::string long2str(long int num);
// c++ 11
std::string llong2str(long long int num);
std::string ullong2str(unsigned long long int num);

#endif  // UTIL_UTIL_H_
