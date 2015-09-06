#ifndef INTERNET_UTIL_H_
#define INTERNET_UTIL_H_

#include <vector>
#include <string>
namespace Util
{
#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))

  int setNonBlock(int fd);

}
#endif //INTERNET_UTIL_H
