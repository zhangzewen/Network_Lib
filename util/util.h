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


#endif  // UTIL_UTIL_H_
