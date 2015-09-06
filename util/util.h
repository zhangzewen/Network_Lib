#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <string>
#include <vector>

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

int daemonize(int nochdir, int noclose);
int setNonBlock(int fd);
std::string lstrip(const std::string& str, std::string decollator = "\f\v\t\n\r ");
std::string rstrip(const std::string& str, std::string decollator = "\f\v\t\n\r ");
std::string strip(std::string& str, std::string decollator = "\f\v\t\n\r ");
void split(const std::string&, const std::string& decollator, std::vector<std::string>& array);

// in c++11 there is a std::to_string can all the things below
std::string to_string(int num);
std::string to_string(long num);
std::string to_string(long long num);
std::string to_string(unsigned num);
std::string to_string(unsigned long num);
std::string to_string(unsigned long long num);
std::string to_string(float num);
std::string to_string(double num);
std::string to_string(long double num);

#endif  // UTIL_UTIL_H_
