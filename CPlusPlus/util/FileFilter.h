#ifndef __NETWORKLIB_UTIL_FILEFILTER_HPP_INCLUDED__
#define __NETWORKLIB_UTIL_FILEFILTER_HPP_INCLUDED__

namespace NetworLib{
namespace Util{

class FileFilter
{
	virtual bool accept(File pathname);
};
}
}

#endif

