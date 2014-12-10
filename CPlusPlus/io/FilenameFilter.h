#ifndef _NETWORKLIB_FILENAMEFILTER_HPP_INCLUDED__
#define _NETWORKLIB_FILENAMEFILTER_HPP_INCLUDED__

namespace Network{
namespace Util{

class FilenameFilter {
	virtual bool accept(File dir, std::string name);
};
}
}


#endif
