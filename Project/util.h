#ifndef _ADAPTOR_UTIL_H_INCLUDED__
#define _ADAPTOR_UTIL_H_INCLUDED__
namespace Util
{
#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))
int setNonBlock(int fd);
}
#endif
